#include "../../includes/minishell.h"

int	create_heredoc(t_redirection *list, t_env *env, int i)
{
	t_share3 *share;
  int sig_flag;

	share = ft_malloc(sizeof(t_share3), 1);
	share->expand_line = NULL;
	share->line = NULL;
	share->fd = -1;
	share->file_name = generate_file_name(env);
	share->fd = open(share->file_name, O_RDWR | O_CREAT | O_TRUNC,  0777);
  if (share->fd == -1)
    protect(env, "Open failed");
	list->heredocs[i] = ft_strdup(share->file_name);
	read_lines(&sig_flag, share, list, env);
	close(share->fd);
	// signal here stop excution
	if (sig_flag)
		return (0);
	return (1);
}

int	open_her(t_tree *tree, t_env *env)
{
	int	i;

	i = 0;
	tree->redirect->heredocs = ft_malloc(20, sizeof(char *));
	ft_memset(tree->redirect->heredocs, 0, sizeof(char *) * 20);
	while (tree->redirect->herdoc)
	{
		if (!create_heredoc(tree->redirect, env, i))
     		return (0);
		tree->redirect->herdoc = tree->redirect->herdoc->next;
		i++;
	}
	tree->redirect->hrc_file = get_last_file(tree->redirect->heredocs);
	return (1);
}

static int	check_heredoc(t_tree *tree)
{
	if (tree->type == REDIRECT_NODE)
	{
		if (tree->redirect->herdoc)
			return (1);
	}
	if (tree->type == PIPE_NODE)
	{
		return (check_heredoc(tree->pipe->left));
		return (check_heredoc(tree->pipe->right));
	}
	return (0);
}

int	open_herdocs(t_tree *tree, t_env *env)
{
	if (!tree)
		return (0);
  if (check_heredoc(tree))
  {
    if (tree->type == REDIRECT_NODE)
    {
      if (!open_her(tree, env))
        return (0);
    }
    else if (tree->type == PIPE_NODE)
    {
      if (!open_herdocs(tree->pipe->left, env))
        return (0);
      if (!open_herdocs(tree->pipe->right, env))
        return (0);
    }
  }
	return (1);
}

// static int handel_heredocs(t_tree *tree, t_env *envp)
// {
//   if (check_heredoc(tree))
//     if (!open_herdocs(tree, envp))
//         return (0);
//   return (1);
// }