#include <stdio.h>
#include <stdint.h>
#include "rbtree.h"

#define NODES      1024*1024 

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;


struct test_node {
	u32 key;
	struct rb_node rb;

	/* following fields used for testing augmented rbtree functionality */
	u32 val;
	u32 augmented;
};

static struct rb_root ROOT = RB_ROOT;
static struct test_node nodes[NODES];

static void insert(struct test_node *node, struct rb_root *root)
{
	struct rb_node **new = &root->rb_node, *parent = NULL;
	u32 key = node->key;

	while (*new) {
		parent = *new;
		if (key < rb_entry(parent, struct test_node, rb)->key)
			new = &parent->rb_left;
		else
			new = &parent->rb_right;
	}

	rb_link_node(&node->rb, parent, new);
	rb_insert_color(&node->rb, root);
}

static inline void erase(struct test_node *node, struct rb_root *root)
{
	rb_erase(&node->rb, root);
}

static void get_rb(u32 key, struct rb_root *root)
{
	struct test_node* rb;
	struct rb_node **new = &root->rb_node;
	struct rb_node* parent;
	
	while(*new)
	{
		parent = *new;
		rb = rb_entry(parent, struct test_node, rb);
		if(key == rb->key)
		{
			printf("key = %d, rb->key = %d \n", key, rb->key);
			printf("val = %d \n", rb->val);
			//erase(rb, &ROOT);
			break;
		}else if(key > rb->key)
		{
			printf("key = %d, rb->key = %d \n", key, rb->key);
			new = &parent->rb_right;	
		}else if(key < rb->key)
		{
			printf("key = %d, rb->key = %d \n", key, rb->key);
			new = &parent->rb_left;
		}else
		{
			break;
			printf("end \n");
		}
	}

}

static void init(void)
{
	int i;

	for (i = 0; i < NODES; i++) {
		nodes[i].key = i+2;
		nodes[i].val = i+100;
	}
}

int main(int argc, char** argv)
{
	int i;

	init();
	for(i = 0; i < NODES; i++)
	{
		insert(nodes+i, &ROOT);
	}

	get_rb(50, &ROOT);
	//insert(&nodes[50], &ROOT);
	get_rb(50, &ROOT);
//	get_rb(89, &ROOT);
//	get_rb(2, &ROOT);
//	get_rb(1, &ROOT);
	return 0;
}
