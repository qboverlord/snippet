
#define MAX 5

bool brute() {

  char startSign = ' ';
  char lastSign = 'z';

  char buf[MAX + 1];
  buf[MAX] = '\0';

  for (unsigned int i = 0; i < MAX; ++i)
    buf[i] = startSign;

  if (checkBuf(buf))
    return true;

  while (buf[MAX - 1] != lastSign) {

    buf[0] += 1;

    if (checkBuf(buf))
      return true;

    if (buf[0] == lastSign) {
      int i = 0;
      while (buf[i] == lastSign) {
        buf[i] = startSign;
        ++i;
      }

      buf[i] += 1;
      if (checkBuf(buf))
        return true;
    }
  }

  for (unsigned int i = 0; i < MAX; ++i)
    buf[i] = lastSign;

  if (checkBuf(buf))
    return true;

  return false;
}
