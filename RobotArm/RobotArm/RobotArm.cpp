#include "GL/glut.h"
#include <math.h>

GLint width = 0, height = 0;
GLdouble p_angle = 0, c_angle = 0;

bool keyPressed[256];//Ű�����Է»�Ȳ�迭
void myKeyboard(unsigned char key, int x, int y) { keyPressed[key] = true; }//�ش�迭�� true�� Key down
void myKeyboardUp(unsigned char key, int x, int y) { keyPressed[key] = false; }//�ش�迭�� false�� Key Up

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
		// math.h�� ����ϱ� ���� angle�� �������� ����
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

void Idle() {//�ش�Ű�� �����ִ��� ���������� �˻��� �����Է��� �Ҽ� �ְ� �Ѵ�
	if (keyPressed['q']) { p_angle += 1; }
	if (keyPressed['a']) { p_angle -= 1; }
	if (keyPressed['w']) { c_angle += 1; }
	if (keyPressed['s']) { c_angle -= 1; }
	else { p_angle = p_angle; c_angle = c_angle;  } //�ƹ��͵� ������ �ʾ������� �̵����ٰ� ���� ��� �𵨺� ����� ���������ش�.
	glutPostRedisplay();// �ٽñ׸���
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
	glutKeyboardFunc(myKeyboard); // Ű�� ������ ��� ����
	glutKeyboardUpFunc(myKeyboardUp); // Ű�� ������ Ǯ�� ��� ����

	glutMainLoop();
	return 0;
}