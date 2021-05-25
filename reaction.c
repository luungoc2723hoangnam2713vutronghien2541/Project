#include "pintos_thread.h"

// Forward declaration. This function is implemented in reaction-runner.c,
// but you needn't care what it does. Just be sure it's called when
// appropriate within reaction_o()/reaction_h().
void make_water();

struct reaction {
	int h_atoms;
	struct condition *new_h_arrival;
	struct condition *reaction_occurred_h;
	struct lock *lck;
};
/*Ham khoi tao*/
void
reaction_init(struct reaction *reaction)
{
	reaction->new_h_arrival = malloc(sizeof(struct condition));
	reaction->reaction_occurred_h = malloc(sizeof(struct condition));
	reaction->lck = malloc(sizeof(struct lock));
	cond_init(reaction->new_h_arrival);
	cond_init(reaction->reaction_occurred_h);
	lock_init(reaction->lck);

	reaction->h_atoms = 0;//Khoi tao so nguyen tu H = 0
}
/* Ham reaction_h(struct reaction *reaction) duoc goi khi nguyen tu H san sang phan ung */
void
reaction_h(struct reaction *reaction)
{
	lock_acquire(reaction->lck);
	reaction->h_atoms++;
	cond_signal(reaction->new_h_arrival,reaction->lck);
	cond_wait(reaction->reaction_occurred_h,reaction->lck);
	lock_release(reaction->lck);
}
/*Ham reaction_o(struct reaction *reaction) duoc goi khi nguyen tu H san sang phan ung. Nguyen tu O phai cho cho đen khi có đu 2 nguyen tu H thi moi thuc hien ham make_water, sau khi thuc hien hàm make_water khong con nguyen tu H. Qua trinh lai thuc hien lai*/
void
reaction_o(struct reaction *reaction)
{
	lock_acquire(reaction->lck);
	while(reaction->h_atoms < 2) {
		cond_wait(reaction->new_h_arrival,reaction->lck);
	}

	//đa đu cac nguyen tu đe tao nen phan tu H2O 
	make_water();
	reaction->h_atoms-=2;
	cond_signal(reaction->reaction_occurred_h,reaction->lck);
	cond_signal(reaction->reaction_occurred_h,reaction->lck);
	lock_release(reaction->lck);
}
