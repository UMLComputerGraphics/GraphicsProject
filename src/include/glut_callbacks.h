/**
 * @file glut_callbacks.h
 * @author John Huston
 * @authors John Huston, Nick St.Pierre, Chris Compton
 * @date 2013-03-13
 * @brief glut_callbacks.h provides function declarations for
 * a set of functions commonly used across multiple binaries
 * for keyboard, mouse and other GLUT callback functions.
 **/

#ifndef __GLUT_CALLBACKS_H
#define __GLUT_CALLBACKS_H

/**
 * keylift is registered as a GLUT callback for when a user
 * releases a depressed key.
 *
 * @param key The key that was lifted.
 * @param x The x coordinate of the mouse at the time the key was released.
 * @param y The y coordinate of the mouse at the time the key was released.
 */
void engineKeylift( unsigned char key, int x, int y );

/**
 * keyboard is a callback registered with GLUT.
 * It handles (surprise!) keyboard input.
 *
 * @param key The key pressed by the user.
 * @param x The x coordinate of the mouse when the key was pressed.
 * @param y The y coordinate of the mouse when the key was pressed.
 */
void engineKeyboard( unsigned char key, int x, int y );

/**
 * keyboard_ctrl is registered as a GLUT callback.
 * It is responsible for catching when special keys are pressed.
 *
 * @param key The key pressed.
 * @param x The x coordinate of the mouse when the key was pressed.
 * @param y The y coordinate of the mouse when the key was pressed.
 */
void engineSpecialKeyboard( int key, int x, int y );

/**
 * mouse is registered as a GLUT callback.
 * It handles input from, primarily, the scrollwheel.
 *
 * @param button The mouse button being pressed.
 * @param state the state of the aforementioned mouse button.
 * @param x the x coordinate of the mouse.
 * @param y the y coordinate of the mouse.
 */
void engineMouse( int button, int state, int x, int y );

/**
 * mouseroll is registered as a GLUT callback.
 * mouseroll is called when the mouse is moved while a button is depressed.
 * It is used here to implement barrel-rolls while left-clicking.
 *
 * @param x the x coordinate of the mouse pointer.
 * @param y the y coordinate of the mouse pointer.
 */
void engineMouseMotion( int x, int y );

/**
 * mouselook is registered as a GLUT callback.
 * mouselook implements FPS-like controls where the camera moves
 * proportional to the direction of the mouse.
 *
 * @param x the x coordinate of the mouse pointer.
 * @param y the y coordinate of the mouse pointer.
 */
void EngineMousePassive( int x, int y );

/**
 * resizeEvent is registered as a glut callback for when
 * the screen is resized. It instructs the screen object
 * of the new size, which informs all of the children cameras
 * to recompute their aspect ratios, viewport positions, and so on.
 *
 * We also warp the pointer to the center of the screen, for
 * compatibility with mouselook( void ).
 *
 * @param width The new width of the window.
 * @param height The new height of the window.
 *
 * @return void.
 */
void engineResize( int width, int height );

void wiilook( Camera &WiiCamera, const Angel::vec3 &NewTheta,
              const Angel::vec3 &MovementRates );

#endif
