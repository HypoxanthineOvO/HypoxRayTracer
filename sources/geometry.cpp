#include "geometry.hpp"
#include <iostream>
// Tiny obj loader
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <omp.h>

bool Triangle::intersect(const Ray& ray, Interaction& interaction) const {
    // Moller Trumbore Algorithm
    Vec3f o = ray.getOrigin(), d = ray.getDirection();
    float tmin = ray.getTMin(), tmax = ray.getTMax();

    Vec3f e1 = v1 - v0, e2 = v2 - v0;
    Vec3f s = o - v0, s1 = d.cross(e2), s2 = s.cross(e1);
    Vec3f ans = (1 / s1.dot(e1)) * Vec3f(s2.dot(e2), s1.dot(s), s2.dot(d));
    float t = ans.x(), u = ans.y(), v = ans.z();

    if (t >= tmin && u >= 0 && v >= 0 && u + v <= 1) {
        interaction.distance = t;
        interaction.position = ray(t);
        interaction.normal = normal.normalized();
        interaction.type = Interaction::InterType::GEOMETRY;
        interaction.material = material;

        return true;
    }
    return false;
}

bool Rectangle::intersect(const Ray& ray, Interaction& interaction) const {
    Vec3f o = ray.getOrigin(), d = ray.getDirection();
    float width = size.x(), height = size.y();

    if (std::abs(d.dot(normal)) <= EPS) {
        return false;
    }
    // Intersect with the plane
    float t = (position - o).dot(normal) / d.dot(normal);
    Vec3f intersect_point = ray(t);
    Vec3f delta_vec = intersect_point - position;

    Vec3f y_tangent = normal.cross(tangent);
    float dw = delta_vec.dot(tangent.normalized()), dh = delta_vec.dot(y_tangent.normalized());

    if (t >= 0 && -width/2 <= dw && dw <= width/2 && -height/2 <= dh && dh <= height/2) {
        interaction.distance = t;
        interaction.position = intersect_point;
        interaction.normal = normal.normalized();
        interaction.type = Interaction::InterType::GEOMETRY;
        interaction.material = material;


        return true;
    }
    return false;
}


bool Ellipsoid::intersect(const Ray& ray, Interaction& interaction) const {
    // Transform to unit sphere
    Mat4f T, R, S;
    T << 1, 0, 0, p.x(),
         0, 1, 0, p.y(),
         0, 0, 1, p.z(),
         0, 0, 0, 1;
    Vec3f normalized_a = a.normalized(), normalized_b = b.normalized(), normalized_c = c.normalized();
    R << normalized_a.x(), normalized_b.x(), normalized_c.x(), 0,
         normalized_a.y(), normalized_b.y(), normalized_c.y(), 0,
         normalized_a.z(), normalized_b.z(), normalized_c.z(), 0,
         0, 0, 0, 1;
    S << a.norm(), 0, 0, 0,
            0, b.norm(), 0, 0,
            0, 0, c.norm(), 0,
            0, 0, 0, 1;
    Mat4f M = T * R * S;
    Mat4f M_inv = M.inverse();

    Vec4f origin_4d = Vec4f(ray.getOrigin().x(), ray.getOrigin().y(), ray.getOrigin().z(), 1),
            direction_4d = Vec4f(ray.getDirection().x(), ray.getDirection().y(), ray.getDirection().z(), 0);
    
    Vec4f origin_4d_transformed = M_inv * origin_4d, direction_4d_transformed = M_inv * direction_4d;

    Vec3f origin_transformed = Vec3f(origin_4d_transformed.x(), origin_4d_transformed.y(), origin_4d_transformed.z()),
            direction_transformed = Vec3f(direction_4d_transformed.x(), direction_4d_transformed.y(), direction_4d_transformed.z());
    

    // Solve the quadratic equation
    float radius = 1;
    float a = direction_transformed.dot(direction_transformed), 
            b = 2 * origin_transformed.dot(direction_transformed), 
            c = origin_transformed.dot(origin_transformed) - radius * radius;
    float delta = b * b - 4 * a * c;
    
    if (delta > 0) {
        float t1 = (-b - sqrtf(delta)) / (2 * a), t2 = (-b + sqrtf(delta)) / (2 * a);
        float t;
        if (t1 > 0 && t2 > 0) {
            t = std::min(t1, t2);
        } 
        else if (t1 > 0) {
            t = t1;
        } 
        else if (t2 > 0) {
            t = t2;
        } 
        else {
            return false;
        }

        if (t < ray.getTMin()){// || t > ray.getTMax()) {
            return false;
        }

        //Mat3f M_inv_transpose = M_inv.block<3, 3>(0, 0).transpose();
        Mat3f M_3 = M.block(0, 0, 3, 3);
        Mat3f M_inv_transpose = M_3.inverse().transpose();
        
        //Vec3f position = ray(t);
        //Vec3f normal = M_inv_transpose * (position - p);

        Vec3f position = origin_transformed + t * direction_transformed;
        Vec3f normal = M_inv_transpose * position;


        interaction.distance = t;
        interaction.position = ray(t);
        interaction.normal = normal.normalized();
        interaction.type = Interaction::InterType::GEOMETRY;
        interaction.material = material;

        return true;
    }
    return false;
}

bool Ground::intersect(const Ray& ray, Interaction& interaction) const {
    float t = (z - ray.getOrigin().z()) / ray.getDirection().z();
    if (t < ray.getTMin()){// || t > ray.getTMax()) {
        return false;
    }
    
    interaction.distance = t;
    interaction.position = ray(t);  
    interaction.normal = Vec3f(0, 0, 1);
    interaction.type = Interaction::InterType::GEOMETRY;
    interaction.material = material;

    return true;
}

Mesh::Mesh(const ObjectConfig& object_config) {
    // Load obj file
    loadObj(object_config.path);
    transformObj(object_config.translate, object_config.scale);

    aabb = AABB(Vec3f(1e8, 1e8, 1e8), Vec3f(-1e8, -1e8, -1e8));
    for (size_t i = 0; i < v_indices.size(); i += 3) {
        Vec3f v0 = vertices[v_indices[i]];
        Vec3f v1 = vertices[v_indices[i+1]];
        Vec3f v2 = vertices[v_indices[i+2]];
        aabb.merge_with(AABB(v0, v1, v2));
    }

    has_accel = object_config.has_acc;
    aabb.Update();
}

void Mesh::loadObj(const std::string& path) {
    /* 
    Load Object filename(end with `.obj`) to this Mesh Object.
    Noticed that this object file have no materials.
    */
    
    // Load obj file
    tinyobj::ObjReaderConfig readerConfig;
    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(path, readerConfig)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }
    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto &attrib = reader.GetAttrib();
    auto &shapes = reader.GetShapes();
    auto &materials = reader.GetMaterials();

    // Generate vertices and normals
    std::vector<Vec3f> vertices, normals;
    std::vector<int> v_indices, n_indices;

    for(size_t i = 0; i < attrib.vertices.size(); i += 3) {
        vertices.push_back(Vec3f(attrib.vertices[i], attrib.vertices[i + 1], attrib.vertices[i + 2]));
    }
    for(size_t i = 0; i < attrib.normals.size(); i += 3) {
        normals.push_back(Vec3f(attrib.normals[i], attrib.normals[i + 1], attrib.normals[i + 2]));
    }
    for(size_t shape_id = 0; shape_id < shapes.size(); shape_id++) {
        size_t index_offset = 0;
        for (size_t face_id = 0; face_id < shapes[shape_id].mesh.num_face_vertices.size(); face_id++) {
            int fv = shapes[shape_id].mesh.num_face_vertices[face_id];
            for (size_t v_id = 0; v_id < fv; v_id++) {
                tinyobj::index_t idx = shapes[shape_id].mesh.indices[index_offset + v_id];
                v_indices.push_back(idx.vertex_index);
                n_indices.push_back(idx.normal_index);
            }
            index_offset += fv;
        }
    }

    // Assign to this object
    this->vertices = vertices;
    this->normals = normals;
    this->v_indices = v_indices;
    this->n_indices = n_indices;

    std::cout << "Vertices: " << vertices.size() << std::endl;
    std::cout << "Normals: " << normals.size() << std::endl;
}

void Mesh::transformObj(Vec3f translation, float scale) {
    // Transform the object
    for (auto& vertex: vertices) {
        vertex = scale * vertex + translation;
    }
}

bool Mesh::intersectTriangle(const Ray& ray, Interaction& interaction,  const Vec3i& v_index, const Vec3i& n_index) const {
    Vec3f v0 = vertices[v_index.x()], v1 = vertices[v_index.y()], v2 = vertices[v_index.z()];
    Vec3f n0 = normals[n_index.x()], n1 = normals[n_index.y()], n2 = normals[n_index.z()];

    Vec3f e1 = v1 - v0, e2 = v2 - v0;
    Vec3f s = ray.getOrigin() - v0, s1 = ray.getDirection().cross(e2), s2 = s.cross(e1);
    Vec3f ans = (1 / s1.dot(e1)) * Vec3f(s2.dot(e2), s1.dot(s), s2.dot(ray.getDirection()));
    float t = ans.x(), u = ans.y(), v = ans.z();


    if (t >= ray.getTMin() && u >= 0 && v >= 0 && u + v <= 1) {
        interaction.distance = t;
        interaction.position = ray(t);
        interaction.normal = ((1 - u - v) * n0 + u * n1 + v * n2).normalized();
        interaction.type = Interaction::InterType::GEOMETRY;
        interaction.material = material;

        return true;
    }
    return false;
}

bool Mesh::intersect(const Ray& ray, Interaction& interaction) const {
    if (has_accel > 0) {
        //printf("Using Acceleration\n");
        gridHit(ray, interaction);
        return interaction.type != Interaction::InterType::NONE;
    }
    else {
        //printf("Not Using Acceleration\n");
        for(int i = 0; i < v_indices.size() / 3; i++) {
            Vec3i v_index(v_indices[3 * i], v_indices[3 * i + 1], v_indices[3 * i + 2]),
                    n_index(n_indices[3 * i], n_indices[3 * i + 1], n_indices[3 * i + 2]);
            Interaction itra;
            if (intersectTriangle(ray, itra, v_index, n_index) && itra.distance < interaction.distance) {
                interaction = itra;
            }
        }
        return interaction.type != Interaction::InterType::NONE;
    }
}

void Mesh::buildAccel(int resolution) {
    auto aabb = getAABB();
    auto grid = std::make_shared<Grid>(resolution);
    grid->init(aabb);

    int cnt = 0;
    #pragma omp parallel for num_threads(48) schedule(dynamic)
    for (int i = 0; i < resolution; i++) {
        printf("\rBuilding Grid: %f %%", 100.0 * cnt / (resolution * resolution * resolution));
        std::cout << std::flush;
        for (int j = 0; j < resolution; j++) {
            for (int k = 0; k < resolution; k++) {
                #pragma omp critical
                cnt++;
                auto& entry = grid->getEntry(i, j, k);
                for(int l = 0; l < v_indices.size() / 3; l++) {
                    Vec3i v_index(v_indices[3 * l], v_indices[3 * l + 1], v_indices[3 * l + 2]);
                    Vec3f v0 = vertices[v_index.x()], v1 = vertices[v_index.y()], v2 = vertices[v_index.z()];
                    if (entry.isOverlapWith(AABB(v0, v1, v2))) {
                        #pragma omp critical
                        grid->addIndex(i, j, k, l);
                    }
                }
            }
        }
    }

    this->accel = grid;
}

void Mesh::gridHit(const Ray& ray, Interaction& interaction) const {
    for (auto entry: accel->entries) {
        if (!entry.isIntersect(ray)) {
            continue;
        }

        for (auto index: entry.indices) {
            Vec3i v_index(v_indices[3 * index], v_indices[3 * index + 1], v_indices[3 * index + 2]),
                    n_index(n_indices[3 * index], n_indices[3 * index + 1], n_indices[3 * index + 2]);
            Interaction itra;
            if (intersectTriangle(ray, itra, v_index, n_index) && itra.distance < interaction.distance) {
                interaction = itra;
            }
        }
    }
}