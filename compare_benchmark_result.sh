awk '
BEGIN {
  std = -1;
  ft = -1;
  has_read_std = 0
  has_read_ft = 0
  RED = "\033[31m";
  RESET = "\033[0m";
  slow_func_idx = 0
}

/^std::/{
  title = $0;
  sub(/^std::/, "", title);
  getline;
  getline;
  if(has_read_std == 0){
    where = match($0, /[0-9]+/)
    end = index($0, "[ns]")
    std = substr($0, where, end - where)
    has_read_std = 1
  }
}
/^ft::/{
  getline;
  getline;
  if(has_read_ft == 0){
    where = match($0, /[0-9]+/)
    end = index($0, "[ns]")
    ft = substr($0, where, end - where)
    has_read_ft = 1
  }
}

{
  if(has_read_std && has_read_ft){
    has_read_std = 0
    has_read_ft = 0
    if (ft <= 0 || std <= 0) {
      printf("速すぎて測れません\n")
    }
    times_slower = ft / std;
    printf("%s: ft: %d, std: %d, ratio: %f\n", title, ft, std, times_slower);
    std=-1;
    ft=-1;
    if (times_slower >= 10) {
      printf("%s%f倍遅い！\n%s", RED, times_slower, RESET)
      slow_funcs[slow_func_idx] = title
      slow_func_idx += 1
    }
  }
}

END {
  printf("%s以下の関数が10倍以上遅かった\n%s", RED, RESET)
  for (i = 0; i < length(slow_funcs); i++) {
    printf("%d. %s\n", i, slow_funcs[i]);
  }
}
'