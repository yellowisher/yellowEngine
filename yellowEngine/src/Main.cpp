using namespace std;

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "yellowEngine/yellowEngine.hpp"
using namespace yellowEngine;

Transform* cameraTransform;
Transform* boxTransform;

float lastX = 1024.0f / 2.0f;
float lastY = 768.0f / 2.0f;
bool firstMouse = true;

float _yaw = 0;
float _pitch = 0;

float speed = 1.0f;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = lastX - (float)xpos;
	float yoffset = lastY - (float)ypos;

	lastX = (float)xpos;
	lastY = (float)ypos;

	_yaw += xoffset * 0.08f;
	_pitch += yoffset * 0.16f;

	cameraTransform->setRotation(_pitch, _yaw, 0);
}

GameObject* g = nullptr;
bool prevPressed = false;
bool pressed = false;

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Vector3 forward = cameraTransform->getForward();
		forward.y = 0;
		forward.normalize();
		cameraTransform->translate(forward * -0.06f*speed);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Vector3 forward = cameraTransform->getForward();
		forward.y = 0;
		forward.normalize();
		cameraTransform->translate(forward * 0.06f*speed);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraTransform->translate(cameraTransform->getRight() * -0.06f*speed);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraTransform->translate(cameraTransform->getRight() * 0.06f*speed);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		cameraTransform->translate(0, -0.06f*speed, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		cameraTransform->translate(0, 0.06f*speed, 0);
	}


	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		boxTransform->translate(boxTransform->getForward()*-0.06f);
	}
	if (glfwGetKey(window, GLFW_KEY_SEMICOLON) == GLFW_PRESS)
	{
		boxTransform->translate(boxTransform->getForward()*0.06f);
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		boxTransform->translate(boxTransform->getRight()*-0.06f);
	}
	if (glfwGetKey(window, GLFW_KEY_APOSTROPHE) == GLFW_PRESS)
	{
		boxTransform->translate(boxTransform->getRight()*0.06f);
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
	{
		boxTransform->rotate(0, 3.0f, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS)
	{
		boxTransform->rotate(0, -3.0f, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
	{
		Vector3 scale = boxTransform->scale;
		scale.x += 0.01f;
		scale.y += 0.01f;
		scale.z += 0.01f;
		boxTransform->setScale(scale);
	}
	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
	{
		Vector3 scale = boxTransform->scale;
		scale.x -= 0.01f;
		scale.y -= 0.01f;
		scale.z -= 0.01f;
		boxTransform->setScale(scale);
	}

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		//if (g == nullptr)
		//{
		//	g = new GameObject("New");
		//	g->addComponent<BoxCollider>();
		//}
		//else
		//{
		//	delete(g);
		//	g = nullptr;
		//}
		pressed = true;
	}
	else
	{
		pressed = false;
	}
}

//void addCube(Transform* target, Mesh* mesh, Material material)
//{
//	GameObject* go = new GameObject();
//	go->addComponent<MeshRenderer>()->set(mesh, material);
//	go->transform->setPosition(target->getWorldPosition());
//
//	auto localPosition = target->position;
//	auto localRotation = target->rotation.toEulerAngle();
//
//	auto worldPosition = target->getWorldPosition();
//	auto worldRotation = target->getWorldRotation().toEulerAngle();
//	auto m = target->getMatrix();
//	auto l = target->getLocalMatrix();
//
//	for (int i = 0; i < target->getChildCount(); i++)
//	{
//		addCube(target->getChild(i), mesh, material);
//	}
//}

int main(void)
{
	System* system = System::getInstance();
	system->setWidth(1024);
	system->setHeight(768);
	system->setResourcePath("./res/");

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1024, 768, "yellow engine", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Shader* textureShader = Shader::create("Shader/texture.vert", "Shader/texture.frag");
	unsigned int lightsIndex = glGetUniformBlockIndex(textureShader->getId(), "LightBlock");
	glUniformBlockBinding(textureShader->getId(), lightsIndex, 0);
	Texture* diffuseMap = Texture::create("Texture/container2.png");
	Texture* specularMap = Texture::create("Texture/container2_specular.png");

	Model* model = Model::create("Mesh/BaseMesh_Anim.fbx");
	//Model* model = Model::create("Mesh/free3DmodelFBX.fbx");
	AnimationClip* clips[3];
	clips[0] = model->getClips().begin()->second;
	clips[1] = (++model->getClips().begin())->second;
	clips[2] = (++model->getClips().begin())->second;

	GameObject* go = model->instantiate("nanosuit");
	go->transform->setScale(0.03f, 0.03f, 0.03f);
	go->transform->rotate(270, 0, 0);
	auto animm = go->addComponent<Animator>();
	//animm->play(clips[0]);

	boxTransform = go->transform->findChild("Root")->findChild("hips")->findChild("thigh.L");
	//boxTransform = go->transform->findChild("free3dmodel_skeleton")->findChild("hips")->findChild("abdomen")->findChild("abdomen2")->findChild("chest")->findChild("shoulder.L");

	Material cubeMaterial(textureShader);
	cubeMaterial.addTexture(diffuseMap, "u_Material.diffuse");
	cubeMaterial.addTexture(specularMap, "u_Material.specular");
	cubeMaterial.setProperty("shininess", 64.0f);

	Mesh* cubeMesh = Mesh::create("Mesh/cube.obj");

	//addCube(go->transform, cubeMesh, cubeMaterial);

	GameObject* body = new GameObject("MovingCube");
	//boxTransform = body->transform;

	GameObject* bodyImage = new GameObject("Body");
	MeshRenderer* cubeRenderer = bodyImage->addComponent<MeshRenderer>()->set(cubeMesh, cubeMaterial);
	bodyImage->transform->setScale(2.0f, 4.0f, 1.0f);

	GameObject* ls = new GameObject("leftShoulder");
	MeshRenderer* lsm = ls->addComponent<MeshRenderer>()->set(cubeMesh, cubeMaterial);
	ls->transform->translate(-1.5f, 1.5f, 0);

	GameObject* rs = new GameObject("rightShoulder");
	MeshRenderer* rsm = rs->addComponent<MeshRenderer>()->set(cubeMesh, cubeMaterial);
	rs->transform->translate(1.5f, 1.5f, 0);

	GameObject* la = new GameObject("leftShoulder");
	MeshRenderer* lam = la->addComponent<MeshRenderer>()->set(cubeMesh, cubeMaterial);
	ls->transform->addChild(la->transform);
	la->transform->setPosition(0, -1.5f, 0);
	la->transform->setScale(1.0f, 2.0f, 1.0f);

	GameObject* ra = new GameObject("rightShoulder");
	MeshRenderer* ram = ra->addComponent<MeshRenderer>()->set(cubeMesh, cubeMaterial);
	rs->transform->addChild(ra->transform);
	ra->transform->setPosition(0, -1.5f, 0);
	ra->transform->setScale(1.0f, 2.0f, 1.0f);

	GameObject* h = new GameObject("rightShoulder");
	h->transform->setPosition(0, 2.5f, 0);

	GameObject* hd = new GameObject("rightShoulder");
	MeshRenderer* hdm = hd->addComponent<MeshRenderer>()->set(cubeMesh, cubeMaterial);
	h->transform->addChild(hd->transform);
	hdm->transform->setPosition(0, 0, 0);

	body->transform->addChild(ls->transform);
	body->transform->addChild(rs->transform);
	body->transform->addChild(h->transform);
	body->transform->addChild(bodyImage->transform);
	Animator* anim = body->addComponent<Animator>();

	AnimationClip* walkingClip = AnimationClip::create("Animation/walking.json");
	AnimationClip* waveClip = AnimationClip::create("Animation/wave.json");

	anim->play(walkingClip);

	GameObject* dl = new GameObject();
	Light* l = dl->addComponent<Light>()->setDirectional();
	l->transform->rotate(0, 180.0f, 0);

	GameObject* dirLightGo = new GameObject("dirLight");
	//Light* light = dirLightGo->addComponent<Light>()->setPoint(1.0f, 0.14f, 0.07f);
	Light* light = dirLightGo->addComponent<Light>()->setDirectional();

	//light->transform->rotate(45.0f, 0, 0);
	light->transform->setScale(0.2f, 0.2f, 0.2f);
	light->transform->setPosition(1.2f, 1.0f, 2.0f);

	GameObject* cameraGo = new GameObject();
	Camera* camera = cameraGo->addComponent<Camera>();
	camera->setPerspective(60.0f, 0.01f, 1000.0f);
	//camera->setPerspective(60.0f, 0.01f, 10000.0f);

	cameraTransform = cameraGo->transform;
	cameraTransform->translate(0, 0, 4);

	ObjectRenderer::_currentCamera = camera;

	animm->play(clips[0]);
	delete(body);
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		if (!prevPressed && pressed)
		{
			animm->play(clips[1]);
		}
		if (prevPressed && !pressed)
		{
			animm->play(clips[0]);
		}
		prevPressed = pressed;

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Light::updateUniformBuffer();
		Animator::proceedAll();
		ObjectRenderer::renderAll(camera);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}