/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
    initShaders();
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.f, 0.f, 1.f, 1.f);
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    freeShaders();
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float angle) {
	//************Tutaj umieszczaj kod rysujący obraz******************l

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float rs = 0.5f;
	float rp = 0.2f;
	float rk = 0.07f;

	float rop = 2.f;
	float rok = 0.3f;

	glm::vec3 op{ 0.f, 0.f, 1.f };
	glm::vec3 ok{ 1.f, 0.f, 0.f };

	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f);
	glm::mat4 V = glm::lookAt(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	spLambert->use();
	glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(V));
	//---Poniższy kawałek kodu powtarzamy dla każdego obiektu

	// slonce
	glm::mat4 Ms = glm::mat4(1.0f);
	Ms = glm::translate(Ms, glm::vec3 { -1.f, 0.f, 0.f });
	//Załadowanie macierzy modelu do programu cieniującego
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(glm::scale(Ms, rs * glm::vec3(1.0f, 1.0f, 1.0f))));
	glUniform4f(spLambert->u("color"), 1, 1, 0, 1);
	Models::sphere.drawSolid(); //Narysowanie obiektu

	// planeta
	glm::mat4 Mp{ Ms };
	Mp = glm::rotate(Mp, angle, glm::vec3(0.0f, 1.0f, 0.0f));
	Mp = glm::translate(Mp, rop * op);
	//Załadowanie macierzy modelu do programu cieniującego
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(glm::scale(Mp, rp * glm::vec3(1.0f, 1.0f, 1.0f))));
	glUniform4f(spLambert->u("color"), 0, 0, 1, 1);
	Models::sphere.drawSolid(); //Narysowanie obiektu

	// ksiezyc
	glm::mat4 Mk{ Mp };
	Mk = glm::rotate(Mk, angle, glm::vec3(0.0f, 1.0f, 0.0f));
	Mk = glm::translate(Mk, rok * ok);
	Mk = glm::scale(Mk, rk * glm::vec3(1.0f, 1.0f, 1.0f));
	//Załadowanie macierzy modelu do programu cieniującego
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(Mk));
	glUniform4f(spLambert->u("color"), 0, 1, 1, 1);
	Models::sphere.drawSolid(); //Narysowanie obiektu

	glfwSwapBuffers(window);
}

float speed{};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) speed = -3.14f;
		if (key == GLFW_KEY_RIGHT) speed = 3.14f;
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT) speed = 0.f;
		if (key == GLFW_KEY_RIGHT) speed = 0.f;
	}
}

int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		fprintf(stderr, "Nie można zainicjować GLAD.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące
	glfwSetKeyCallback(window, key_callback);

	float angle = 0;
	glfwSetTime(0);
	//Główna pętla
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		angle += speed * glfwGetTime();
		glfwSetTime(0);
		drawScene(window, angle); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}