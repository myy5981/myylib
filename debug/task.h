#ifndef MYY_TASK_H
#define MYY_TASK_H

typedef struct {
	int (*before)(void);
	void (*task)(void);
	void (*after)(void);
}Task;

extern const Task tasks[];
extern int tasks_len;

#endif
