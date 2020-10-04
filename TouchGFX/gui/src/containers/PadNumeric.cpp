#include <gui/containers/PadNumeric.hpp>
#include <string.h>

#define KEY_1		'1'
#define KEY_2		'2'
#define KEY_3		'3'
#define KEY_4		'4'
#define KEY_5		'5'
#define KEY_6		'6'
#define KEY_7		'7'
#define KEY_8		'8'
#define KEY_9		'9'
#define KEY_0		'0'
#define KEY_DECIMAL '.'

PadNumeric::PadNumeric()
{
	position = 0;
}

void PadNumeric::initialize()
{
    PadNumericBase::initialize();
}


void PadNumeric::Btn1Click()
{
	if ((TEXTAREA_SIZE-1) > position)
	{
		textAreaBuffer[position++] = KEY_1;
		textAreaBuffer[position] = 0;
		textArea.invalidate();
	}
}

void PadNumeric::Btn2Click()
{
	if ((TEXTAREA_SIZE-1) > position)
	{
		textAreaBuffer[position++] = KEY_2;
		textAreaBuffer[position] = 0;
		textArea.invalidate();
	}
}

void PadNumeric::Btn3Click()
{
	if ((TEXTAREA_SIZE-1) > position)
	{
		textAreaBuffer[position++] = KEY_3;
		textAreaBuffer[position] = 0;
		textArea.invalidate();
	}
}

void PadNumeric::Btn4Click()
{
	if ((TEXTAREA_SIZE-1) > position)
	{
		textAreaBuffer[position++] = KEY_4;
		textAreaBuffer[position] = 0;
		textArea.invalidate();
	}
}

void PadNumeric::Btn5Click()
{
	if ((TEXTAREA_SIZE-1) > position)
	{
		textAreaBuffer[position++] = KEY_5;
		textAreaBuffer[position] = 0;
		textArea.invalidate();
	}
}

void PadNumeric::Btn6Click()
{
	if ((TEXTAREA_SIZE-1) > position)
	{
		textAreaBuffer[position++] = KEY_6;
		textAreaBuffer[position] = 0;
		textArea.invalidate();
	}
}

void PadNumeric::Btn7Click()
{
	if ((TEXTAREA_SIZE-1) > position)
	{
		textAreaBuffer[position++] = KEY_7;
		textAreaBuffer[position] = 0;
		textArea.invalidate();
	}
}

void PadNumeric::Btn8Click()
{
	if ((TEXTAREA_SIZE-1) > position)
	{
		textAreaBuffer[position++] = KEY_8;
		textAreaBuffer[position] = 0;
		textArea.invalidate();
	}
}

void PadNumeric::Btn9Click()
{
	if ((TEXTAREA_SIZE-1) > position)
	{
		textAreaBuffer[position++] = KEY_9;
		textAreaBuffer[position] = 0;
		textArea.invalidate();
	}
}

void PadNumeric::Btn0Click()
{
	if ((TEXTAREA_SIZE-1) > position)
	{
		textAreaBuffer[position++] = KEY_0;
		textAreaBuffer[position] = 0;
		textArea.invalidate();
	}
}

void PadNumeric::BtnDecimalClick()
{
	if ((TEXTAREA_SIZE-1) > position)
	{
		textAreaBuffer[position++] = KEY_DECIMAL;
		textAreaBuffer[position] = 0;
		textArea.invalidate();
	}
}

void PadNumeric::BtnDeleteClick()
{
	if (position > 0)
	{
		position--;
		textAreaBuffer[position] = 0;
		textArea.invalidate();
	}
}

int PadNumeric::get_buffer_length()
{
	return TEXTAREA_SIZE;
}

void PadNumeric::get_buffer(uint8_t *buffer)
{
	memset(buffer, 0, TEXTAREA_SIZE*sizeof(uint8_t));
	Unicode::toUTF8(textAreaBuffer, buffer, TEXTAREA_SIZE);
}
