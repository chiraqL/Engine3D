#pragma once

#include "Model.h"

class Renderer
{
private:
	Camera* camera = nullptr;	// pointer to a camera
	Light* light = nullptr;		// pointer to light object containing array of all types of lights
	Model* model = nullptr;		// pointer to model to render
public:
	Renderer() { init(); }
	~Renderer() { close(); }
public:
	void init() {
		camera = new Camera();
		light = new Light(0.1f);						// pass Ia -> ambient intensity for surrounding
		model = new Model("3D-Cottage.obj", light);		// load obj file, add model based light sources

		// one directional light source for all meshes
		light->sun = new Sun( {1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f,0.0f,500.0f}, {0,0,-1} ); // 
	}
	void update() {
		// change view -> front, top, left, right
		if (!glb.view.isDefault())
		{
			model->rotAngle.x = 0;
			model->rotAngle.y = 0;
			model->rotAngle.z = 0;
			if (glb.view.l)
				model->rotAngle.y = -90;
			else if (glb.view.r)
				model->rotAngle.y = 90;
			else if (glb.view.t)
				model->rotAngle.x = 90;
		}
		mat4 t = math::translate(model->position);
		mat4 rx = math::rotatex(math::radians(model->rotAngle.x));
		mat4 ry = math::rotatey(math::radians(model->rotAngle.y));
		mat4 rz = math::rotatez(math::radians(model->rotAngle.z));
		mat4 s = math::scale(model->scaleFactor);
		mat4 m;
		m = t * rz * rx * ry * s;			// model transformation matrix

		camera->forwardDir = math::normalize(camera->lookDir * glb.tElapsed);
		camera->target = { 0.0f,0.0f,-1.0f };
		camera->lookDir = math::toVec3(math::rotatey(math::radians(glb.camYaw)) * camera->target);
		camera->target = camera->eye + camera->lookDir;
		mat4 v = math::lookAt(camera);		// view matrix
		mat4 mv = v * m;					// model view matrix

		float fov = 90.0f;
		mat4 p = glb.controls.projectP ? math::perspective(fov, (float)glb.draw.vp_width / glb.draw.vp_height, 0.1f, 100.0f) : math::orthogonal();	// projection matrix

		// transform lights as model does
		// sun
		light->sun->position = math::sphToCart(light->sun->translateSph);
		light->sun->tfPosition = m * light->sun->position;
		vec4 newPos = v * light->sun->tfPosition;
		light->sun->setDirection(camera->target, newPos);
		// dlights
		// ...
		// plights
		// ...
		model->draw(camera, light, mv, p);
	}
	void close() {
		delete light->sun;
		delete model;
		delete light;
		delete camera;
	}
	Model* getModel() { return model; }
	Camera* getCamera() { return camera; }
	Light* getLight() { return light; }
};

