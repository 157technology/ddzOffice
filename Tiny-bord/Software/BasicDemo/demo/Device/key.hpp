#ifndef __KEY_H__
#define __KEY_H__
#ifdef __cplusplus
#include "signalslot.hpp"

class Key : public Object
{
public:
	Key();
	~Key() {}

enum KeyState{PRESSED = 0, UNPRESSED, RELEASED, CLICKED};
private:
	/* key left */
	int m_leftPressTimeCount;
	enum KeyState m_leftState;

	/* key right */
	int m_rightPressTimeCount;
	enum KeyState m_rightState;

public:

	/* 仅用于底层ISR */
	void isr_handle(void);

	/* signal for Key left */
	void leftClicked(void) {};
	void emit_leftClicked(void) {};

	void leftPressed(void) {};
	void emit_leftPressed(void) {};

	void leftReleased(void) {};
	void emit_leftReleased(void) {};

	/* signal for Key right */
	void rightClicked(void) {};
	void emit_rihgtClicked(void) {};

	void rightPressed(void) {};
	void emit_rightPressed(void) {};

	void rightReleased(void) {};
	void emit_rightReleased(void) {};
};
#endif
#endif