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

#include <GL/glew.h>
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

void planety(float angle)
{
	float rs = 0.5f;
	float rp = 0.2f;
	float rk = 0.07f;

	float rop = 2.f;
	float rok = 0.3f;

	glm::vec3 op{ 0.f, 0.f, 1.f };
	glm::vec3 ok{ 1.f, 0.f, 0.f };

	//---Poniższy kawałek kodu powtarzamy dla każdego obiektu

	// slonce
	glm::mat4 Ms = glm::mat4(1.0f);
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
	//Załadowanie macierzy modelu do programu cieniującego
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(glm::scale(Mk, rk * glm::vec3(1.0f, 1.0f, 1.0f))));
	glUniform4f(spLambert->u("color"), 0, 1, 1, 1);
	Models::sphere.drawSolid(); //Narysowanie obiektu

	// planeta2
	glm::mat4 Mp1{ Ms };
	Mp1 = glm::rotate(Mp1, angle / 2.f, glm::vec3(1.0f, 1.0f, 0.0f));
	Mp1 = glm::translate(Mp1, 0.6f * rop * op);
	//Załadowanie macierzy modelu do programu cieniującego
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(glm::scale(Mp1, rp * glm::vec3(1.0f, 1.0f, 1.0f))));
	glUniform4f(spLambert->u("color"), 1, 0, 1, 1);
	Models::sphere.drawSolid(); //Narysowanie obiektu

	// ksiezyc2
	glm::mat4 Mk2{ Mp1 };
	Mk2 = glm::rotate(Mk2, angle * 2.f, glm::vec3(0.0f, 1.0f, 0.0f));
	Mk2 = glm::translate(Mk2, rok * ok);
	//Załadowanie macierzy modelu do programu cieniującego
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(glm::scale(Mk2, rk * glm::vec3(1.0f, 1.0f, 1.0f))));
	glUniform4f(spLambert->u("color"), 0, 1, 1, 1);
	Models::sphere.drawSolid(); //Narysowanie obiektu
}

void torusy(float angle)
{
	glm::vec3 osObrotu{ 0,0,1 };
	glm::vec3 srodek1{ 1,0,0 };
	glm::vec3 srodek2{ -1,0,0 };

	glm::mat4 M1{ 1.f };
	M1 = glm::translate(M1, srodek1);
	M1 = glm::rotate(M1, angle, osObrotu);
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M1));
	glUniform4f(spLambert->u("color"), 0, 1, 1, 1);
	Models::torus.drawSolid();

	for (int i = 0; i < 12; i++)
	{
		glm::mat4 Mk{ M1 };
		Mk = glm::rotate(Mk, (float)glm::radians(i * 30.f), osObrotu);
		Mk = glm::translate(Mk, glm::vec3{ 1,0,0 });
		Mk = glm::scale(Mk, glm::vec3{ 0.1, 0.1, 0.1 });
		glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(Mk));
		glUniform4f(spLambert->u("color"), 0, 1, 0, 1);
		Models::cube.drawSolid();
	}

	glm::mat4 M2{ 1.f };
	M2 = glm::translate(M2, srodek2);
	M2 = glm::rotate(M2, -angle, osObrotu);
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M2));
	glUniform4f(spLambert->u("color"), 1, 0, 1, 1);
	Models::torus.drawSolid();

	for (int i = 0; i < 12; i++)
	{
		glm::mat4 Mk{ M2 };
		Mk = glm::rotate(Mk, (float)glm::radians(i * 30.f) + (float)glm::radians(15.f), osObrotu);
		Mk = glm::translate(Mk, glm::vec3{ 1,0,0 });
		Mk = glm::scale(Mk, glm::vec3{ 0.1, 0.1, 0.1 });
		glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(Mk));
		glUniform4f(spLambert->u("color"), 0, 1, 0, 1);
		Models::cube.drawSolid();
	}
}

void torusy2(float angle)
{
	glm::vec3 osObrotu{ 0,0,1 };
	glm::vec3 srodek1{ 2.1f,0,0 };

	for (int j = 0; j < 6; j++)
	{
		glm::mat4 M1{ 1.f };
		M1 = glm::scale(M1, glm::vec3{ 0.5, 0.5, 0.5 });
		M1 = glm::rotate(M1, (float)glm::radians(j * 360.f / 6.f), osObrotu);
		M1 = glm::translate(M1, srodek1);
		M1 = glm::rotate(M1, angle, osObrotu);
		glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M1));
		glUniform4f(spLambert->u("color"), 0, 1, 1, 1);
		Models::torus.drawSolid();

		for (int i = 0; i < 12; i++)
		{
			glm::mat4 Mk{ M1 };
			Mk = glm::rotate(Mk, (float)glm::radians(i * 30.f) + j * (float)glm::radians(15.f), osObrotu);
			Mk = glm::translate(Mk, glm::vec3{ 1,0,0 });
			Mk = glm::scale(Mk, glm::vec3{ 0.1, 0.1, 0.1 });
			glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(Mk));
			glUniform4f(spLambert->u("color"), 0, 1, 0, 1);
			Models::cube.drawSolid();
		}
		angle = -angle;
	}
}

void torusy3(float angle)
{
	glm::vec3 osObrotu{ 0,0,1 };
	glm::vec3 srodek1{ 2.1f,0,0 };

	for (int j = 0; j < 6; j++)
	{
		glm::mat4 M1{ 1.f };
		M1 = glm::scale(M1, glm::vec3{ 0.5, 0.5, 0.5 });
		M1 = glm::rotate(M1, (float)glm::radians(j * 360.f / 6.f), glm::vec3{ 0,1,0 });
		M1 = glm::translate(M1, srodek1);
		M1 = glm::rotate(M1, (float)glm::radians(45.f), glm::vec3{ 0,1,0 });
		M1 = glm::rotate(M1, angle, osObrotu);
		glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M1));
		glUniform4f(spLambert->u("color"), 0, 1, 1, 1);
		Models::torus.drawSolid();

		for (int i = 0; i < 12; i++)
		{
			glm::mat4 Mk{ M1 };
			Mk = glm::rotate(Mk, (float)glm::radians(i * 30.f) + j * (float)glm::radians(15.f), osObrotu);
			Mk = glm::translate(Mk, glm::vec3{ 1,0,0 });
			Mk = glm::scale(Mk, glm::vec3{ 0.1, 0.1, 0.1 });
			glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(Mk));
			glUniform4f(spLambert->u("color"), 0, 1, 0, 1);
			Models::cube.drawSolid();
		}
		angle = -angle;
	}
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float angle) {
	//************Tutaj umieszczaj kod rysujący obraz******************l

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f);
	glm::mat4 V = glm::lookAt(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	spLambert->use();
	glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(V));

	torusy3(angle);

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

	window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
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
