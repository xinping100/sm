#pragma once

struct share_memory_message
{
	int type;
	union
	{
		struct 
		{
			int key;
			int obj_pool_id;
		} new_object;

		struct 
		{
			int key;
			int obj_pool_id;
		} del_object;
	};
};

enum share_memory_message_type
{
	smmt_begin = 1,
	smmt_new_object,
	smmt_delete_object,
	smmt_end,
};
