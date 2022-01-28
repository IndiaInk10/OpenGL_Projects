#include "GL/glut.h"
#include <math.h>

GLint width = 0, height = 0;
GLdouble p_angle = 0, c_angle = 0;

bool keyPressed[256];//키보드입력상황배열
void myKeyboard(unsigned char key, int x, int y) { keyPressed[key] = true; }//해당배열이 true면 Key down
void myKeyboardUp(unsigned char key, int x, int y) { keyPressed[key] = false; }//해당배열이 false면 Key Up

void DrawRobotArm()
{
	glColor3f(0.0, 1.0, 0.0);

	glPushMatrix();
		glRotatef(p_angle, 0, 0, 1);
		glLineWidth(10);
		glBegin(GL_LINES);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(10.0, 0.0, 0.0);
		glEnd();
	glPopMatrix();
	glPushMatrix();
		float ang = 3.14159265 / 180 * p_angle; 
		// math.h를 사용하기 위해 angle을 라디안으로 변경
		glTranslatef(10 * cos(ang), 10 * sin(ang), 0);
		glRotatef(c_angle, 0, 0, 1);
		glLineWidth(10);
		glBegin(GL_LINES);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(10, 0.0, 0.0);
		glEnd();
	glPopMatrix();
}

void myView()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		glOrtho(-20, 20, -20, 20, 5, -1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			gluLookAt(0, 0, 5, 0, 0, -1, 0, 1, 0);
			DrawRobotArm();
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	
	glutSwapBuffers();
}

void Idle() {//해당키가 눌려있는지 지속적으로 검사해 다중입력을 할수 있게 한다
	if (keyPressed['q']) { p_angle += 1; }
	if (keyPressed['a']) { p_angle -= 1; }
	if (keyPressed['w']) { c_angle += 1; }
	if (keyPressed['s']) { c_angle -= 1; }
	else { p_angle = p_angle; c_angle = c_angle;  } //아무것도 눌리지 않았을때는 이동없다고 보고 계속 모델뷰 행렬을 유지시켜준다.
	glutPostRedisplay();// 다시그리기
}

void MyReshap(int NewWidth, int NewHeight) {
	if (NewHeight == 0)
		NewHeight = 1;

	width = NewWidth;
	height = NewHeight;
}

int main(int argc, char** argv)
{
	width = 600, height = 600;

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	glutInitWindowSize(width, height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Robot Arm");

	glClearColor(0.0, 0.0, 0.0, 1.0);

	glutDisplayFunc(myView);
	glutReshapeFunc(MyReshap);
	glutIdleFunc(Idle);
	glutKeyboardFunc(myKeyboard); // 키가 눌렸을 경우 실행
	glutKeyboardUpFunc(myKeyboardUp); // 키가 눌림이 풀린 경우 실행

	glutMainLoop();
	return 0;
}