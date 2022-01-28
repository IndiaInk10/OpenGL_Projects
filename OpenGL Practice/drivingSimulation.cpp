#include <gl/glut.h>

#define VIEWY 10

GLdouble viewZ = 0, viewX = 0, RViewZ = 0, RViewX = 0; // ī�޶� �̵���Ű�� ����
GLint width, height; // â ũ�⿡ ���� ����Ʈ ������ �� ����Ʈ ���� �ϱ� ���� ����
GLfloat handleRotateD = 0, rotate = 0; // �ڵ��� ���ư� ������ ��Ÿ���� ����, �ڵ��� ���ư� ������ ��Ÿ���� ����
GLdouble vf = 0.5, vf0 = 0, af = 0.5, tf = 0, vb = -0.5, vb0 = 0, ab = -0.5, tb = 0;
// ��ӵ� ��� �ϱ� ���� ����ϴ� ������ v �ӵ�/ v0 �ʱ�ӵ�/ a ���ӵ�/ t �ð�(f : forward, b : backward)

void pressKey(int key, int x, int y);
void releaseKey(int key, int x, int y);

void DrawFloor() { // ���� �ٴ� ����
	glColor3f(0.0, 1.0, 1.0);

	GLfloat FloorRows = 200.0, FloorCols = 200.0;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j <= 40; j++)
		{
			if (i == 0)
			{
				glPushMatrix();
					glLineWidth(3);
					glBegin(GL_LINES);
						glVertex3f(FloorRows, 0.0, FloorCols);
						glVertex3f(FloorRows, 0.0, -FloorCols);
					glEnd();
				glPopMatrix();
				FloorRows -= 10.0;
			}
			else
			{
				glPushMatrix();
					glLineWidth(3);
					glBegin(GL_LINES);
						glVertex3f(FloorRows, 0.0, FloorCols);
						glVertex3f(-FloorRows, 0.0, FloorCols);
					glEnd();
				glPopMatrix();
				FloorCols -= 10.0;
			}
		}
	}
}
	
void DrawHandle() { // �ڵ� ����
	glColor3f(1.0, 1.0, 1.0);

	glPushMatrix();
		glTranslatef(viewX + 3.0, 7.0, viewZ + 5.0); // ī�޶� ���� �̵�
		glRotatef(40, 1, 0, 0);
		glRotatef(rotate, 0, 0, 1); // �ڵ��� ������ Rotate ��ȯ
		glScalef(1.2, 1.2, 1.2);
		glutWireTorus(0.3, 1.0, 3, 8);
	glPopMatrix();
}

void handleRotate(int angle)
{
	if (handleRotateD == 90 && rotate <= 90) // ���������� ������ ���
	{
		angle += 3;
		rotate = angle;
		glutPostRedisplay();
		glutTimerFunc(25, handleRotate, angle);
	}
	else if (handleRotateD == -90 && rotate >= -90) // ����
	{
		angle -= 3;
		rotate = angle;
		glutPostRedisplay();
		glutTimerFunc(25, handleRotate, angle);
	}
}

void handleReverseRotate(int angle)
{
	if (handleRotateD == 90 && rotate >= 0) // ���������� ���ȴ� �ڵ��� ���� ���
	{
		rotate -= 3;
		angle = rotate;
		glutPostRedisplay();
		glutTimerFunc(25, handleReverseRotate, angle);
	}
	else if (handleRotateD == -90 && rotate <= 0) // ����
	{
		rotate += 3;
		angle = rotate;
		glutPostRedisplay();
		glutTimerFunc(25, handleReverseRotate, angle);
	}
}

void ForwardAcceleration(int MaxV) // ������ ��ӵ� �
{
	if (vf <= MaxV)
	{
		vf0 = vf;
		vf = vf0 + af * tf;
		viewZ = viewZ + vf;
		tf += 0.05;
		glutPostRedisplay();
		glutTimerFunc(25, ForwardAcceleration, MaxV);
	}
}
void BackwardAcceleration(int MaxV) // �ڷ� ��ӵ� �
{
	if (vb >= MaxV)
	{
		vb0 = vb;
		vb = vb0 + ab * tb;
		viewZ = viewZ + vb;
		tb += 0.05;
		glutPostRedisplay();
		glutTimerFunc(25, BackwardAcceleration, MaxV);
	}
}

void MyWorld()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, width / 6, height); // ���� ���̵� �̷�
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		gluPerspective(40, 3, 1, 70);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			gluLookAt(viewX, VIEWY, viewZ, -(viewX + RViewX), 0, -((viewZ + 30) + RViewZ), 0.0, 1.0, 0.0);
			DrawFloor();
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	

	glViewport(width / 6, 0, (width / 6) * 4, (height / 4) * 3); // ������ �þ�
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		gluPerspective(40, 3, 1, 70);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			gluLookAt(viewX, VIEWY, viewZ, viewX + RViewX, 0, (viewZ + 30) + RViewZ, 0.0, 1.0, 0.0);
			DrawFloor();
			DrawHandle();
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	

	glViewport((width / 6) * 5, 0, width / 6, height); //  ������ ���̵� �̷�
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		gluPerspective(40, 3, 1, 70);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			gluLookAt(viewX, VIEWY, viewZ, -(viewX + RViewX), 0, -((viewZ + 30) + RViewZ), 0.0, 1.0, 0.0);
			DrawFloor();
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	

	glViewport(width / 6, (height / 4) * 3, (width / 6) * 4, height / 4); //  �� �̷�
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		gluPerspective(40, 3, 1, 70);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			gluLookAt(viewX, VIEWY, viewZ, -(viewX + RViewX), 0, -((viewZ + 30) + RViewZ), 0.0, 1.0, 0.0);
			DrawFloor();
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	

	glutSwapBuffers();
}

void pressKey(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		handleRotateD = -90;
		glutTimerFunc(25, handleRotate, 0);
		break;
	case GLUT_KEY_RIGHT:
		handleRotateD = 90;
		glutTimerFunc(25, handleRotate, 0);
		break;
	case GLUT_KEY_UP:
		glutTimerFunc(25, ForwardAcceleration, 5);
		vf = 0, tf = 0;
		break;
	case GLUT_KEY_DOWN:
		glutTimerFunc(25, BackwardAcceleration, -5);
		vb = 0, tb = 0;
		break;
	}
}

/*==========================================================================*/

void releaseKey(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		glutTimerFunc(25, handleReverseRotate, rotate);
		break;
	case GLUT_KEY_RIGHT:
		glutTimerFunc(25, handleReverseRotate, rotate);
		break;
	case GLUT_KEY_UP:
		//vf = 0;
		break;
	case GLUT_KEY_DOWN:
		//vb = 0;
		break;
	}
}

void MyReshap(int NewWidth, int NewHeight) {
	if (NewHeight == 0)
		NewHeight = 1;

	width = NewWidth;
	height = NewHeight;
}

int main(int argc, char** argv) {
	width = 1000; height = 600;

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(0, 0);	
	glutCreateWindow("Driving Simulation");

	glClearColor(0.0, 0.0, 0.0, 1.0);

	glutIgnoreKeyRepeat(1); // �ڵ� �ݺ� �Է��� �����ִ� �Լ�(Ű�� release�� �˱� ����)
	glutSpecialFunc(pressKey); // Ű�� ������ ��� ����
	glutSpecialUpFunc(releaseKey); // Ű�� ������ Ǯ�� ��� ����

	glutDisplayFunc(MyWorld);
	glutReshapeFunc(MyReshap);
	glutMainLoop();

	return 0;
}