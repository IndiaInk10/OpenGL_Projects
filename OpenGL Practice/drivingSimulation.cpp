#include <gl/glut.h>

#define VIEWY 10

GLdouble viewZ = 0, viewX = 0, RViewZ = 0, RViewX = 0; // 카메라를 이동시키는 변수
GLint width, height; // 창 크기에 따른 뷰포트 사이즈 및 뷰포트 분할 하기 위한 변수
GLfloat handleRotateD = 0, rotate = 0; // 핸들이 돌아간 방향을 나타내는 변수, 핸들이 돌아간 정도를 나타내는 변수
GLdouble vf = 0.5, vf0 = 0, af = 0.5, tf = 0, vb = -0.5, vb0 = 0, ab = -0.5, tb = 0;
// 등가속도 운동을 하기 위해 사용하는 변수들 v 속도/ v0 초기속도/ a 가속도/ t 시간(f : forward, b : backward)

void pressKey(int key, int x, int y);
void releaseKey(int key, int x, int y);

void DrawFloor() { // 격자 바닥 생성
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
	
void DrawHandle() { // 핸들 생성
	glColor3f(1.0, 1.0, 1.0);

	glPushMatrix();
		glTranslatef(viewX + 3.0, 7.0, viewZ + 5.0); // 카메라를 따라 이동
		glRotatef(40, 1, 0, 0);
		glRotatef(rotate, 0, 0, 1); // 핸들을 돌리는 Rotate 변환
		glScalef(1.2, 1.2, 1.2);
		glutWireTorus(0.3, 1.0, 3, 8);
	glPopMatrix();
}

void handleRotate(int angle)
{
	if (handleRotateD == 90 && rotate <= 90) // 오른쪽으로 돌리는 경우
	{
		angle += 3;
		rotate = angle;
		glutPostRedisplay();
		glutTimerFunc(25, handleRotate, angle);
	}
	else if (handleRotateD == -90 && rotate >= -90) // 왼쪽
	{
		angle -= 3;
		rotate = angle;
		glutPostRedisplay();
		glutTimerFunc(25, handleRotate, angle);
	}
}

void handleReverseRotate(int angle)
{
	if (handleRotateD == 90 && rotate >= 0) // 오른쪽으로 돌렸다 핸들을 놓은 경우
	{
		rotate -= 3;
		angle = rotate;
		glutPostRedisplay();
		glutTimerFunc(25, handleReverseRotate, angle);
	}
	else if (handleRotateD == -90 && rotate <= 0) // 왼쪽
	{
		rotate += 3;
		angle = rotate;
		glutPostRedisplay();
		glutTimerFunc(25, handleReverseRotate, angle);
	}
}

void ForwardAcceleration(int MaxV) // 앞으로 등가속도 운동
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
void BackwardAcceleration(int MaxV) // 뒤로 등가속도 운동
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

	glViewport(0, 0, width / 6, height); // 왼쪽 사이드 미러
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
	

	glViewport(width / 6, 0, (width / 6) * 4, (height / 4) * 3); // 운전자 시야
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
	

	glViewport((width / 6) * 5, 0, width / 6, height); //  오른쪽 사이드 미러
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
	

	glViewport(width / 6, (height / 4) * 3, (width / 6) * 4, height / 4); //  백 미러
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

	glutIgnoreKeyRepeat(1); // 자동 반복 입력을 막아주는 함수(키의 release를 알기 위함)
	glutSpecialFunc(pressKey); // 키가 눌렸을 경우 실행
	glutSpecialUpFunc(releaseKey); // 키가 눌림이 풀린 경우 실행

	glutDisplayFunc(MyWorld);
	glutReshapeFunc(MyReshap);
	glutMainLoop();

	return 0;
}