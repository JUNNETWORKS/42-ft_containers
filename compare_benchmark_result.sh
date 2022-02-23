awk '
BEGIN {
  std = 0;
  ft = 0;
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
    printf("%s\n", title);
    printf("\tft: %d, std: %d, ", ft, std)
    if (ft <= 0 || std <= 0) {
      printf("速すぎて測れません\n")
    } else {
      times_slower = ft / std;
      printf("ratio: %f\n", times_slower)
      std=-1;
      ft=-1;
      if (times_slower >= 10) {
        printf("\t%s%f倍遅い！%s\n", RED, times_slower, RESET)
        slow_funcs[slow_func_idx] = title
        slow_func_idx += 1
      }
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