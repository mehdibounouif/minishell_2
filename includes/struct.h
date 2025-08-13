/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 01:55:26 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/13 02:32:25 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

# include <stddef.h>

typedef struct s_command		t_command;
typedef struct s_redirection	t_redirection;
typedef struct s_tree			t_tree;
typedef struct s_node			t_node;
typedef struct s_pipe			t_pipe;
typedef struct s_end			t_end;
typedef struct s_env			t_env;
typedef struct s_mini			t_mini;
typedef struct s_herdoc			t_herdoc;
typedef struct s_files			t_files;

struct							s_env
{
	char						*key;
	char						*value;
	struct s_env				*next;
};

struct							s_node
{
	int							contain_quoted;
	int							between_quoted;
	int							type;
	int							b_space;
	char						*content;
	struct s_node				*next;
	struct s_node				*prev;
};

struct							s_pipe
{
	t_tree						*left;
	t_tree						*right;
};

struct							s_command
{
	char						*command;
	char						**args;
};

struct							s_herdoc
{
	char						*file;
	char						*herdoc;
	char						*delimeter;
	int							quoted;
	struct s_herdoc				*next;
};

struct							s_files
{
	int							type;
	char						*file;
	struct s_files				*next;
};

struct							s_redirection
{
	t_tree						*prev;
	char						*in_file;
	char						**in_files;
	char						*out_file;
	char						**out_files;
	char						**heredocs;
	char						*hrc_file;
	int							in;
	int							hdc;
	int							in_count;
	int							out_count;
	int							out_type;
	t_files						*files;
	t_herdoc					*herdoc;
};

struct							s_tree
{
	int							type;
	int							ret;
	t_command					*command;
	t_pipe						*pipe;
	t_redirection				*redirect;
};

struct							s_mini
{
	t_tree						*tree;
	t_node						*list;
};

typedef struct s_gcollect
{
	void						*value;
	struct s_gcollect			*next;
}								t_gcollect;

// expantion struct
typedef struct t_share
{
	int							i;
	int							j;
	int							h;
	char						*expanded_cmd;
}								t_share;

typedef struct s_share4
{
	t_tree						*redirect_node;
	t_tree						*prev;
	t_node						*tmp;
	int							i;
	int							j;
}	t_share4;

typedef struct s_share1
{
	t_node	*tmp;
	t_node	*next;
	t_node	*prev;
	int		flag;

}	t_share1;

typedef struct s_share2
{
	int		i;
	size_t	full_len;
	char	*value;
	char	*key;

}	t_share2;

typedef struct s_expand
{
	t_node	*start;
	t_node	*next;
	t_node	*sub_list;
	t_node	*new_list;
	t_node	*to_free;
	t_node	*tmp;

}	t_expand;

#endif
