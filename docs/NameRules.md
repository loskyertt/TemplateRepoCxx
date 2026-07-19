# 1. 核心命名法（字面写法）

C++ 中主要混合使用以下三种命名法，根据**代码元素的种类**来区分：

1. **蛇形命名法 / 下划线命名法**：单词之间用下划线连接，通常全小写。
    * *示例*：`my_variable`, `compute_value()`
2. **大驼峰命名法 / 帕斯卡命名法**：每个单词首字母大写，无分隔符。
    * *示例*：`MyClass`, `HttpRequest`
3. **小驼峰命名法**：第一个单词首字母小写，后续单词首字母大写。
    * *示例*：`myVariable`, `getValue`

---

# 2. 具体代码元素的命名规范（重点）

C++ 命名的核心哲学是：**看名字的格式，就能猜出它是什么东西（变量、函数、类型还是宏）。**

## 2.1 宏定义与枚举值：全大写 + 下划线 (SCREAMING_SNAKE_CASE)

这是从 C 语言继承来的传统，用于区分预处理宏和普通变量。

* **宏定义**：`#define MAX_BUFFER_SIZE 1024`
* **枚举值**：`enum Color { RED, DARK_GREEN }` *(注：C++11 引入的 enum class 有时也允许用大驼峰，但全大写依然最普遍)*

## 2.2 模板参数：大驼峰 或 全大写

现代 C++ 越来越倾向于使用大驼峰，以区分模板参数和宏定义；有些旧代码仍用全大写。

* **现代推荐**：`template <typename Type, int Size>`
* **传统写法**：`template <typename T, int N>` (T, N 这种单字母在简单模板中很常见)

## 2.3 类、结构体、枚举、类型别名：大驼峰

所有自定义类型的名字，首字母均大写，以此与变量名区分。如 `uint64_t` 是系统底层C风格，但现代 C++ 自己定义 class 时多用大驼峰。

* **类/结构体**：`class UserProfile`, `struct NodeData`
* **枚举类**：`enum class HttpStatus`
* **类型别名**：`using HashMap = std::unordered_map<std::string, int>;`
* **概念 (C++20)**：`SortableContainer`

## 2.4 变量与成员变量：全小写 + 下划线

这是 C++ 社区（尤其是 Google 规范）最主流的做法，变量名全部小写。

**【极其重要的争议点：成员变量的前缀】**
为了在函数内部区分局部变量和类的成员变量，通常需要给成员变量加前缀或后缀，常见有三大流派：

* **下划线后缀派 (Google 规范推荐)**：`int count_;` (推荐，不会与系统保留前缀冲突)
* **m_ 前缀派 (MFC/Qt 等老牌框架常用)**：`int m_count;` (m 代表 member)
* **无标记派 (靠 this-> 区分)**：`int count;` (赋值时写 `this->count = count;`)

* **全局变量**：极少使用，若使用通常加 `g_` 前缀，如 `g_instance`。
* **静态变量**：通常加 `s_` 前缀，如 `s_singleton`。

## 2.5 函数与函数对象：全小写 + 下划线 vs 大驼峰

这是 C++ 社区**分歧最大**的地方，两派各有道理，关键是在一个项目中保持统一：

* **流派 A：全小写 + 下划线 (Google 规范 / C++ Core Guidelines 推荐)**
    * *理由*：C++ 标准库（STL）全部使用下划线（如 `std::sort`, `vector.push_back()`）。遵循 STL 风格，代码看起来更地道。
    * *示例*：`void compute_average();`, `int get_size() const;`
* **流派 B：大驼峰**
    * *理由*：函数是动作，首字母大写可以立刻和全小写的变量区分开，阅读时不需要依赖上下文。
    * *示例*：`void ComputeAverage();`, `int GetSize() const;`

---

# 3. 语义命名原则

除了拼写格式，名字的**含义**同样重要。

1. **名副其实**：不要用 `a`, `b`, `c`（除非是简单的循环变量 `i`, `j`, `k` 或数学公式中的变量 `x`, `y`），也不要用 `data`,
   `info` 这种含糊的词。
    * ❌ `int d;` (消逝的时间？距离？)
    * ✅ `int elapsed_days;`
2. **函数名应当是动词或动宾结构**：
    * ✅ `remove_prefix()`, `draw_circle()`, `is_empty()`
3. **布尔变量/函数用 is/has/can/should 开头**：
    * ✅ `bool is_visible;`, `bool has_children();`
4. **Getter/Setter 命名**：
    * 如果用下划线流派：getter 通常与变量同名 `int count() const;`，setter 加 set_ 前缀 `void set_count(int c);`
    * 如果用大驼峰流派：`int Count() const;`, `void SetCount(int c);`

---

# 4. C/C++ 保留规则

这些是 C++ 标准明确保留的，**你在任何情况下都不应该使用的命名**，否则会导致未定义行为或编译错误：

1. **双下划线**：任何包含双下划线的名字都被保留（如 `my__var`, `hello__world`）。
2. **下划线 + 大写字母开头**：如 `_Var`, `_Count`，被保留给编译器和标准库内部使用。
3. **全局/命名空间作用域的下划线开头**：如 `_global_var`，被保留给 C 标准库。
4. **`_t` 后缀**：如前所述，这是 POSIX 标准保留用于类型定义的，不要在自己的业务代码中定义 `xxx_t`。

---

# 5. 总结：推荐的新项目模板

如果你要启动一个现代 C++ 项目，推荐采用类似 Google 风格的统一标准：

| 代码元素                   | 命名风格                                                            | 示例                                                    |
|:-----------------------|:----------------------------------------------------------------|:------------------------------------------------------|
| **宏 / 枚举值**            | SCREAMING_SNAKE_CASE                                            | `MAX_SIZE`, `COLOR_RED`                               |
| **类 / 结构体 / 类型**       | PascalCase                                                      | `UserData`, `HttpServer`                              |
| **函数**                 | snake_case                                                      | `process_request()`, `get_id()`                       |
| **普通变量**               | snake_case                                                      | `user_count`, `buffer_size`                           |
| **类成员变量**              | 前缀 `m` + snake_case                                             | `m_user_count`, `m_name`                              |
| **常量 / 全局变量 / static** | 前缀 `k` + snake_case / 前缀 `g` + snake_case / 前缀 `s` + snake_case | `k_max_buffer_size` / `g_default_name` / `s_instance` |
| **模板参数**               | PascalCase                                                      | `typename KeyType`                                    |

> 💡 **终极原则**：**一致性重于一切**。无论你选择哪种规范，请让整个团队从头到尾严格遵守，并使用 `clang-format` 和 `clang-tidy` 等工具自动化检查。