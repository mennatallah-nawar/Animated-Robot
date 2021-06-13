# Texture Mapping, Object Loading and Animation

Objects and Animation Part: <br>
We thought of a way to make the scene more realistic, 
so we made Robot like a child playing with ball and cheering when
make goal, using 3 objects : ball, football-goal and cap 
then we display them in dispaly function and add animation to them using Timer and Setposes Functions

```<language>
void setPoses(int frameNum) {
	ball = poses[frameNum][0];

	RightShoulder = poses[frameNum][1];
	RightElbow = poses[frameNum][2];

	LeftShoulder = poses[frameNum][3];
	LeftElbow = poses[frameNum][4];

	LeftLeg = poses[frameNum][5];
	RightLeg = poses[frameNum][6];

	RightKnee = poses[frameNum][7];
	LeftKnee = poses[frameNum][8];
}
static int f = 0;
void timer(int value)
{
	f = f % 5;
	setPoses(f);
	f++;
	glutPostRedisplay();
	glutTimerFunc(1500, timer, 0);
	
}
```
* We faced problem when we add Cap object to the scene the cap.
Cap color effect on the color of all objecte so we make some adjustments on Cap TML file.


Floor Part: <br>

Finding .bmp file wasn’t easy so we converted .jpeg images into .bmp on a website. The different images had different sizes which made it hard to make them appear in the same way on the floor. <tr> 

We used the loadTexture function to get the texture id and set the image width and height. <tr> 

The function main menu was divided to 4 if conditions, each condition to show an image when clicked. <tr> 
``` <language> 

if (value == 1) 

    { 

        Image* image = loadBMP("images/Panda.bmp"); 

        _textureId = loadTexture(image); 

        delete image; 

               glutPostRedisplay(); 

    } 
``` 

In the initRendering we added the lighting and the main image that shows on the floor when we open the window before use the menu to change it. 

```<language> 
 Image* image = loadBMP("images/rocks.bmp"); 

    _textureId = loadTexture(image); 

    delete image; 
``` 

In the display function, we added the floor and the texture and started trying different coordinates so the images could appear properly. 
```<language> 
  glPushMatrix(); 

    glTranslatef(0, -6, -10); 

    glScaled(25, 7, 10); 

    glPushMatrix(); 

    glEnable(GL_TEXTURE_2D); 

    glBindTexture(GL_TEXTURE_2D, _textureId); 

  

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

  

    glBegin(GL_QUADS); 

    glNormal3f(0.0, 1.0, 0.0); 

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1, -1, 1); 

    glTexCoord2f(2.0f, 0.0f); glVertex3f(1, -1, 1); 

    glTexCoord2f(2.0f, 2.0f); glVertex3f(1, -1, -1); 

    glTexCoord2f(0.0f, 2.0f); glVertex3f(-1, -1, -1); 

    glEnd(); 

    glDisable(GL_TEXTURE_2D); 

    glPopMatrix(); 

    glPopMatrix(); 

``` 
Last but not least, in the main function we added the titles of the main menu and showed that it will appear upon right click.
```<language>
    glutCreateMenu(main_menu); 

    glutAddMenuEntry("Panda Texture", 1); 

    glutAddMenuEntry("Rocks Texture", 2); 

    glutAddMenuEntry("magic Texture", 3); 

    glutAddMenuEntry("field Texture", 4); 

    glutAttachMenu(GLUT_RIGHT_BUTTON); 
```
	
![image](https://github.com/mennatallah-nawar/Animated-Robot/blob/master/Animation.gif)
