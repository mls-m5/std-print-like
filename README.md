# format-like

## Example usage

```c++
#include "print.h"

int main(int argc, char *argv[]) {
    print("hello {} you\n", "there");

    auto str = format("hello {} you\n", "there");
    print("{}", str);
    
    print("x: {}, y: {}", 10, 20);

    return 0;
}
```
