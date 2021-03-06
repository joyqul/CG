#include <iostream>
#include <cmath>
#include "scene.h"

scene::scene(const char* path, const char* file) {
    this->path = path;
    this->file_name = file;
    Init();
}

scene::~scene() {
    for (int i = 0, j = this->object.size(); i < j; ++i)
        (this->object)[i].~model();
}

float scene::GetFraction() {
    float dis[] = {this->eye[0] - this->vat[0], this->eye[2] - this->vat[2]};
    float r = sqrt(dis[0]*dis[0] + dis[1]*dis[1]);
    if (r < 0.1f) return r/70;
    return r/700;
}

void scene::Forward(float x, float z) {
    this->eye[0] += x;
    this->eye[2] += z;
    this->vat[0] += x;
    this->vat[2] += z;
}

void scene::Backward(float x, float z) {
    this->eye[0] -= x;
    this->eye[2] -= z;
    this->vat[0] -= x;
    this->vat[2] -= z;
}

void scene::Left(float x, float z) {
    this->eye[0] -= x;
    this->eye[2] -= z;
    this->vat[0] -= x;
    this->vat[2] -= z;
}

void scene::Right(float x, float z) {
    this->eye[0] += x;
    this->eye[2] += z;
    this->vat[0] += x;
    this->vat[2] += z;
}

void scene::TurnLeft(float  fraction, float x, float z) {
    float dis[] = {this->eye[0] - this->vat[0], this->eye[2] - this->vat[2]};
    float r = sqrt(dis[0]*dis[0] + dis[1]*dis[1]);
    this->eye[0] += fraction * x;
    this->eye[2] += fraction * z;
    float new_dis[] = {this->eye[0] - this->vat[0], this->eye[2] - this->vat[2]};
    float new_r = sqrt(new_dis[0]*new_dis[0] + new_dis[1]*new_dis[1]);
    this->eye[0] = new_dis[0]*r/new_r + this->vat[0];
    this->eye[2] = new_dis[1]*r/new_r + this->vat[2];
}

void scene::TurnRight(float  fraction, float x, float z) {
    float dis[] = {this->eye[0] - this->vat[0], this->eye[2] - this->vat[2]};
    float r = sqrt(dis[0]*dis[0] + dis[1]*dis[1]);
    this->eye[0] -= fraction * x;
    this->eye[2] -= fraction * z;
    float new_dis[] = {this->eye[0] - this->vat[0], this->eye[2] - this->vat[2]};
    float new_r = sqrt(new_dis[0]*new_dis[0] + new_dis[1]*new_dis[1]);
    this->eye[0] = new_dis[0]*r/new_r + this->vat[0];
    this->eye[2] = new_dis[1]*r/new_r + this->vat[2];
}

void scene::TurnUp(float fraction, float cross_x, float cross_y, float cross_z) {
    float dis[] = {this->eye[0] - this->vat[0], this->eye[1] - this->vat[1], this->eye[2] - this->vat[2]};
    float r = sqrt(dis[0]*dis[0] + dis[1]*dis[1] + dis[2]*dis[2]);
    this->eye[0] += fraction * vup[0];
    this->eye[1] += fraction * vup[1];
    this->eye[2] += fraction * vup[2];
    float new_dis[] = {this->eye[0] - this->vat[0], this->eye[1] - this->vat[1], this->eye[2] - this->vat[2]};
    float new_r = sqrt(new_dis[0]*new_dis[0] + new_dis[1]*new_dis[1] + new_dis[2]*new_dis[2]);
    float ratio = r/new_r;
    
    this->eye[0] *= ratio;
    this->eye[1] *= ratio;
    this->eye[2] *= ratio;
    
    // Fixed vup
    dis[0] = this->eye[0] - this->vat[0];
    dis[1] = this->eye[1] - this->vat[1]; 
    dis[2] = this->eye[2] - this->vat[2];

    float new_vup_x = dis[1]*cross_z - dis[2]*cross_y;
    float new_vup_y = dis[2]*cross_x - dis[0]*cross_z;
    float new_vup_z = dis[0]*cross_y - dis[1]*cross_x;
    float new_vup_dis = sqrt(new_vup_x*new_vup_x + new_vup_y*new_vup_y + new_vup_z*new_vup_z);
    this->vup[0] = new_vup_x/new_vup_dis; 
    this->vup[1] = new_vup_y/new_vup_dis; 
    this->vup[2] = new_vup_z/new_vup_dis; 
}

void scene::TurnDown(float fraction, float cross_x, float cross_y, float cross_z) {
    float dis[] = {this->eye[0] - this->vat[0], this->eye[1] - this->vat[1], this->eye[2] - this->vat[2]};
    float r = sqrt(dis[0]*dis[0] + dis[1]*dis[1] + dis[2]*dis[2]);
    this->eye[0] -= fraction * vup[0];
    this->eye[1] -= fraction * vup[1];
    this->eye[2] -= fraction * vup[2];
    float new_dis[] = {this->eye[0] - this->vat[0], this->eye[1] - this->vat[1], this->eye[2] - this->vat[2]};
    float new_r = sqrt(new_dis[0]*new_dis[0] + new_dis[1]*new_dis[1] + new_dis[2]*new_dis[2]);
    float ratio = r/new_r;
    
    this->eye[0] *= ratio;
    this->eye[1] *= ratio;
    this->eye[2] *= ratio;
    
    // Fixed vup
    dis[0] = this->eye[0] - this->vat[0];
    dis[1] = this->eye[1] - this->vat[1]; 
    dis[2] = this->eye[2] - this->vat[2];

    float new_vup_x = dis[1]*cross_z - dis[2]*cross_y;
    float new_vup_y = dis[2]*cross_x - dis[0]*cross_z;
    float new_vup_z = dis[0]*cross_y - dis[1]*cross_x;
    float new_vup_dis = sqrt(new_vup_x*new_vup_x + new_vup_y*new_vup_y + new_vup_z*new_vup_z);
    this->vup[0] = new_vup_x/new_vup_dis; 
    this->vup[1] = new_vup_y/new_vup_dis; 
    this->vup[2] = new_vup_z/new_vup_dis; 
}

void scene::LoadModel() {
    // Load models
    FILE* scene;
    std::string scene_file = this->path;
    scene_file += this->file_name;
    scene_file += ".scene";
    scene = fopen(scene_file.c_str(), "r");

	if (!scene) {
        std::cout<< "Can not open scene object File \"" << scene_file << "\" !" << std::endl;
		return;
	}

    char type[20], obj_file[50];
    while (fscanf(scene, "%s", type) != EOF) {
        
        if (!strcmp(type, "model")) {
            fscanf(scene, "%s", obj_file);
            std::string object_file = this->path;
            object_file += obj_file;
            model* object_tmp = new model(this->path, object_file.c_str());
            fscanf(scene, "%f%f%f%f%f%f%f%f%f%f", &object_tmp->Sx, &object_tmp->Sy, &object_tmp->Sz, 
                    &object_tmp->Angle, &object_tmp->Rx, &object_tmp->Ry, &object_tmp->Rz, 
                    &object_tmp->Tx, &object_tmp->Ty, &object_tmp->Tz);
            (this->object).push_back(*object_tmp);
        }
    }
    if (scene) fclose(scene);
}

void scene::LoadView() {
    // Load view
    FILE* scene;
    std::string scene_file = this->path;
    scene_file += this->file_name;
    scene_file += ".view";
    scene_file = "TestScene/assignment3.view";
    scene = fopen(scene_file.c_str(), "r");

	if (!scene) {
        std::cout<< "Can not open scene view File \"" << scene_file << "\" !" << std::endl;
		return;
	}
    
    std::cout << "Load view file:\"" << scene_file << "\"" << std::endl;

    char type[20];
    while (fscanf(scene, "%s", type) != EOF) {
        
        if (!strcmp(type, "eye")) {
            fscanf(scene, "%f%f%f", &this->eye[0], &this->eye[1], &this->eye[2]);
        }
        else if (!strcmp(type, "vat")) {
            fscanf(scene, "%f%f%f", &this->vat[0], &this->vat[1], &this->vat[2]);
        }
        else if (!strcmp(type, "vup")) {
            fscanf(scene, "%f%f%f", &this->vup[0], &this->vup[1], &this->vup[2]);
        }
        else if (!strcmp(type, "fovy")) {
            fscanf(scene, "%f", &this->fovy);
        }
        else if (!strcmp(type, "dnear")) {
            fscanf(scene, "%f", &this->dnear);
        }
        else if (!strcmp(type, "dfar")) {
            fscanf(scene, "%f", &this->dfar);
        }
        else if (!strcmp(type, "viewport")) {
            fscanf(scene, "%f%f%f%f", &this->viewport[0], &this->viewport[1], 
                    &this->viewport[2], &this->viewport[3]);
        }
    }
    if (scene) fclose(scene);
}

void scene::LoadLight() {
    FILE* scene;
    std::string scene_file = this->path;
    scene_file += this->file_name;
    scene_file += ".light";
    scene_file = "TestScene/assignment3.light";
    scene = fopen(scene_file.c_str(), "r");
	if (!scene) {
        std::cout<< "Can not open scene view File \"" << scene_file << "\" !" << std::endl;
		return;
	}
    
    std::cout << "Load light file:\"" << scene_file << "\"" << std::endl;

    char type[20];
    while (fscanf(scene, "%s", type) != EOF) {
        
        if (!strcmp(type, "light")) {
            light* light_tmp = new light();
            fscanf(scene, "%d%d%d%f%f%f%f%f%f%f%f%f", 
                    &(light_tmp->x), &(light_tmp->y), &(light_tmp->z), 
                    &(light_tmp->ar), &(light_tmp->ag), &(light_tmp->ab), 
                    &(light_tmp->dr), &(light_tmp->dg), &(light_tmp->db), 
                    &(light_tmp->sr), &(light_tmp->sg), &(light_tmp->sb));
            (this->lights).push_back(*light_tmp);
        }
        else if (!strcmp(type, "ambient")) {
            fscanf(scene, "%f%f%f", &this->ambient[0], &this->ambient[1], &this->ambient[2]);
        }
    }
    if (scene) fclose(scene);
}

void scene::Init() {
    LoadModel();
    LoadView();
    LoadLight();
}

void scene::LightTop() {
    this->lights[0].x += vup[0];
    this->lights[0].y += vup[1];
    this->lights[0].z += vup[2];
}

void scene::LightDown() {
    this->lights[0].x -= vup[0];
    this->lights[0].y -= vup[1];
    this->lights[0].z -= vup[2];
}

void scene::LightLeft() {
    float fraction = GetFraction()*5;
    float dis[] = {this->eye[0] - this->vat[0], 
                   this->eye[1] - this->vat[1],
                   this->eye[2] - this->vat[2]};
    float normal[] = {dis[1]*this->vup[2] - dis[2]*this->vup[1],
                      dis[2]*this->vup[0] - dis[0]*this->vup[2],
                      dis[0]*this->vup[1] - dis[1]*this->vup[0]};
    this->lights[0].x += normal[0]*fraction;
    this->lights[0].y += normal[1]*fraction;
    this->lights[0].z += normal[2]*fraction;
}
void scene::LightRight() {
    float fraction = GetFraction()*5;
    float dis[] = {this->eye[0] - this->vat[0], 
                   this->eye[1] - this->vat[1],
                   this->eye[2] - this->vat[2]};
    float normal[] = {dis[1]*this->vup[2] - dis[2]*this->vup[1],
                      dis[2]*this->vup[0] - dis[0]*this->vup[2],
                      dis[0]*this->vup[1] - dis[1]*this->vup[0]};
    this->lights[0].x -= normal[0]*fraction;
    this->lights[0].y -= normal[1]*fraction;
    this->lights[0].z -= normal[2]*fraction;
}
model::model(const char* path, const char* obj_file) {
    this->mesh_object = new mesh(path, obj_file);
}

model::~model() {
}
