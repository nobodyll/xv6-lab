struct VMA {
  int index;
  int used;
  void *addr;
  int length;
  int prot;           // r w x
  int flags;          // SHARED PRIVATE
  struct file *file;  
  int fd;
};