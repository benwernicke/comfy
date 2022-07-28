# comfy
comfy helper stuff

## todo
Todo is a simple todo list. You can print your todos with `todo`, add a todo
with `todo add <head> <description>` and remove a todo with `todo rm
<head/number>`.

### Installation
You should alter the `#define TODO_FILE_PATH ""` based on your local setup
after that run:
```
cd todo
gcc cbuild.c -o cbuild
./cbuild fast
./cbuild install
```
