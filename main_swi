#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

/*
 * redirection처리시 남은 부분. 
 * 1. 구조체(이름모름)에서 받은 값을 red_open_file, red_pre_execute 잘 처리해주기
 * 2. 에러 제대로 출력.
 * 3. 출력값 redirection 처리
 * 4. redirection 처리하면서 만든 fd들 close해줘야함. 
 * 4-1. 마지막 in 아니면 바로 close. here_doc & in은 execute 후에 바로 close. 
 * 4-2. 마지막 out 아니면 바로 close.
 * 5. stdout 따로 저장해줘야할까? 그냥 쓰면 될까?
 *
 * 출력은 3가지 경우로 나눔.
 * 우선 출력용 fd 하나를 두고, 그 fd값에 red~ 또는 pipe를 넣어주고
 * stdout이랑 연결
 * 파이프와 red~가 없다면 dup_check 없이 바로 stdout
 */

int main(int argc, char *argv[], char *env[])
{
	t_envs	e;
	int		fd;
	int		i;
	t_paths	p;
	t_oper	o;
	char	*input_oper;
	int		pipe_fd[2];
	int		status;

	if (input_env(&e, env) == 1)
		return (1);
	// 여기서부터 redirection 부분.

	/* >> 시스템 함수 실행시 pipe close()할 때를 위해 init() */
	//init_pipe(pipe_fd);
	/* >> 만약 pipe가 있으면 미리 여기서 pipe 생성 후 out redirection 연결*/
	//make_pipe(pipe_fd);
	/* >> pipe가 없으면 stdout 다시 제대로 연결 */
	//dup_check(STDOUT_FILENO, STDOUT_FILENO);

	/* >> redirection open && dup2 && close(fd) */
	//if (red_open_file(T_RE_HEREDOC, "a") == WRONG_ACTION)
		/*error check 후에 종료 또는 다음 pipe 실행*/

	find_path(&(p.paths), &(p.max_len));
	input_oper = argv[1];
	o.oper_path = make_oper(&o.opers, p.max_len, p.paths, input_oper);
	
	// 파이프 유무 체크 후 확인.
	// 파이프가 있으면 cd 도 동작 안함.(이동하지 않음)
	// 즉 파이프가 있으면 무조건 fork해서 실행된다.
	
	// 아래 함수 부터 수정하면 됨.
	/*===========================================================*/
	/*
	 * if (파이프 있으면)
	 * 		자식 생성 함수
	 * else
	 * 		if 빌트인 함수면
	 * 			빌트인 함수 실행
	 * 		else
	 * 			시스템 함수 실행
	 */
	/*===========================================================*/
	if (built_in_check(&o, &e) == -1)
	{
		if (o.oper_path == NULL)
			printf("bash: %s: %s\n", o.opers[0], COMMAND_ERROR); // exit:127
		else
			exe_oper(&o, pipe_fd, e.env);
	}
	/*===========================================================*/
	while (waitpid(-1, &status, 0) >= 0)
		;
	// 만약 다음 파이프가 없고 status == 11 이라면 아래의 값 출력
		//printf("Segmentation fault: %d\n", status);
		//exit code: 128+11=139
	free_double_char(o.opers);// 매 번들마다.
	free(o.oper_path);//매 번들마다.

	free_envs(&e);//bash 종료시에.
	free_double_char(p.paths);// bash 종료시에.
	return (0);
}
