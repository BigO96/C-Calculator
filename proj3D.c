#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define QUEUE_SIZE 10
#define STACK_SIZE 10

typedef enum
{
	ADD,
	MULT,
	SUBTRACT,
	UNSUPPORTED
} MathOperation;

void IssueBadNumberError()
{
	printf("The string does not represent a floating point number.\n");
	exit(EXIT_FAILURE);
}

void IssueBadOperationError()
{
	printf("The string does not represent a valid operation.\n");
	exit(EXIT_FAILURE);
}
// START STACK
typedef struct
{
	double items[STACK_SIZE];
	int top;
} Stack;

void InitializeStack(Stack *s)
{
	s->top = -1;
}

void Push(Stack *s, double element)
{
	if (s->top == STACK_SIZE - 1)
	{
		printf("ERROR!\n");
		return;
	}
	s->items[++s->top] = element;
}

double Pop(Stack *s)
{
	return s->items[s->top--];
}
// END STACK

// START QUEUE
typedef struct
{
	int front, back, size;
	char **array;
} Queue;

void InitializeQueue(Queue *q)
{
	q->array = malloc(sizeof(char *) * QUEUE_SIZE);
	q->front = 0;
	q->back = 0;
	q->size = 0;

	for (int i = 0; i < QUEUE_SIZE; ++i)
	{
		q->array[i] = NULL;
	}
}

void Enqueue(Queue *q, char *str)
{
	if (q->size == QUEUE_SIZE)
		printf("ERROR!\n");
	q->back = (q->back) % QUEUE_SIZE;
	q->array[q->back] = str;
	q->size++;
	q->back++;
}

char *Dequeue(Queue *q)
{
	if (q->size == 0)
		return NULL;

	char *item = q->array[q->front];
	q->front = (q->front + 1) % QUEUE_SIZE;
	q->size--;
	// q->back--;
	return item;
}
// END QUEUE

MathOperation GetOperation(char *op)
{
	if (op[1] != '\0')
	{
		return UNSUPPORTED;
	}
	switch (op[0]) /* Test */
	{
	case '+':
		return ADD;
	case '-':
		return SUBTRACT;
	case 'x':
		return MULT;
	default:
		return UNSUPPORTED;
	}
}

double StringToDouble(const char *str)
{
	double result = 0.0;
	int bool_negative = 0;
	int bool_decimal = 0;
	double decimalValue = 0.1;

	if (*str == '-')
	{
		bool_negative = 1;
		str++;
	}

	while (*str)
	{
		if (*str == '.')
		{
			bool_decimal++;
			str++;
			continue;
		}

		int i = *str - '0';
		if (i < 0 || i > 9)
		{
			IssueBadNumberError();
		}

		if (bool_decimal == 0)
		{
			result = (result * 10) + i;
		}
		else
		{
			result += i * decimalValue;
			decimalValue /= 10;
		}
		if (bool_decimal > 1)
		{
			IssueBadNumberError();
		}
		str++;
	}

	if (bool_negative == 1)
	{
		result = -result;
	}
	if (bool_decimal > 1)
	{
		IssueBadNumberError();
	}
	return result;
}

double calculate(Queue *q, Stack *s)
{
	char *a;
	while ((a = Dequeue(q)) != NULL)
	{
		MathOperation op = GetOperation(a);

		if (op == UNSUPPORTED)
		{
			double num = StringToDouble(a);
			Push(s, num);
		}
		else
		{
			double num2 = Pop(s);
			double num1 = Pop(s);

			switch (op)
			{
			case ADD:
				Push(s, num1 + num2);
				break;
			case SUBTRACT:
				Push(s, num1 - num2);
				break;
			case MULT:
				Push(s, num1 * num2);
				break;
			}
		}
	}
	return Pop(s);
}

int main(int argc, char *argv[])
{
	Queue q;
	InitializeQueue(&q);

	Stack s;
	InitializeStack(&s);

	for (int i = 1; i < argc; i++)
	{
		Enqueue(&q, argv[i]);
	}

	double result = calculate(&q, &s);
	printf("The total is %f\n", result);

	return 0;
}
