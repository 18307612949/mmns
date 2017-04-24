struct LL {
	void	  *data;
	struct LL *next;
	struct LL *prev;
};


inline struct LL *get_next(struct LL *head);
void free_whole_list(struct LL *head);
void print_list(struct LL *head);
void print_list_reverse(struct LL *head);
void *add_node(struct LL *head, void *data);
void *trim_list(struct LL *head, long after);
