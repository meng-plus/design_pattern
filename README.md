---
文档: 数据模式
版本: V1.00-20250124
---

# **设计模式**

## 观察者模式 (Observer Pattern)
### **1. 概述**
观察者模式是一种行为型设计模式，适用于解决一对多的通知问题。其核心思想是定义一种订阅机制，使得对象之间实现松耦合，当主题对象（Subject）的状态发生变化时，所有依赖它的观察者对象（Observer）都会收到通知并自动更新。

在实际编程设计中，这种模式经常用于前后端数据同步、实时事件通知等场景。例如：
- 收到数据后，通知 UI 刷新界面。
- 设备状态变化后，通知相关模块处理。
- 实现日志模块、事件监听等功能。

这种模式允许底层模块专注于核心逻辑，不需要预留具体的上层接口，通过观察者机制可以灵活地添加新的响应逻辑。

---

### **2. 优势与应用场景**
#### **优势**
- **松耦合**：主题与观察者之间的依赖关系仅限于通知机制，彼此无需了解具体实现。
- **动态扩展**：可以在运行时动态添加或移除观察者，无需修改主题代码。
- **事件驱动**：通过事件通知实现模块间的实时通信。

#### **应用场景**
- **UI 刷新**：数据状态变化时触发界面更新。
- **设备状态监控**：设备离线、数据异常等事件通知。
- **事件驱动框架**：实现订阅-发布模式的消息系统。

---

### **3. 观察者模式在 C 语言中的实现**

在 C 语言中，由于缺少类和对象的概念，观察者模式的实现需要依赖结构体和函数指针，同时避免了堆内存（heap）的开销。本例提供了一个轻量级的观察者模式实现，适合嵌入式场景。

#### **核心结构体**
##### **1. 数据基类**
```c
typedef struct OBSERVER_DATA_BASE
{
    uint32_t type; /*!< 数据类型标识 */
} observer_data_t;
```
数据基类 `observer_data_t` 用于定义数据类型，具体使用时可以根据实际需求继承并扩展。

##### **2. 观察者**
```c
typedef struct Observer
{
    void (*update)(const struct Observer *); /*!< 更新回调函数 */
    observer_data_t *data;                   /*!< 通知的数据 */
    mm_list_t node;                          /*!< 链表节点 */
} Observer;
```
`Observer` 定义了观察者接口，包括：
- `update`：观察者的更新函数，主题通知时会调用此函数。
- `data`：存储通知的数据。
- `node`：链表节点，用于组织观察者链表。

##### **3. 主题**
```c
typedef struct Subject
{
    mm_list_t observers;   /*!< 观察者链表 */
    observer_data_t *data; /*!< 当前主题的数据 */
} Subject;
```
`Subject` 定义了主题接口，包含：
- `observers`：维护的观察者链表。
- `data`：主题的状态数据。

---

#### **函数接口**
##### **1. 注册观察者**
```c
void subject_register_observer(Subject *subject, Observer *observer);
```
将观察者 `observer` 注册到主题 `subject` 的观察者链表中。

##### **2. 移除观察者**
```c
void subject_remove_observer(Subject *subject, Observer *observer);
```
从主题 `subject` 中移除观察者 `observer`。

##### **3. 通知观察者**
```c
void subject_notify_observers(Subject *subject);
```
遍历主题 `subject` 的观察者链表，调用每个观察者的 `update` 函数，实现通知。

##### **4. 设置主题状态并通知观察者**
```c
void subject_set_data(Subject *subject, observer_data_t *data);
```
设置主题 `subject` 的数据状态 `data`，并触发观察者通知。

---

### **4. 使用示例**

以下是一个观察者模式的使用示例：

```c
#include "observer.h"

// 自定义数据类型
typedef struct
{
    observer_data_t base;
    int value; /*!< 数据值 */
} custom_data_t;

// 自定义观察者更新函数
void custom_update(const Observer *observer)
{
    custom_data_t *data = (custom_data_t *)observer->data;
    printf("Observer notified. Data value: %d\n", data->value);
}

int main()
{
    // 创建主题
    Subject subject = {0};
    mm_list_init(&subject.observers);

    // 创建观察者
    Observer observer = {0};
    observer.update = custom_update;

    // 创建数据
    custom_data_t data = {{.type = 1}, .value = 42};

    // 注册观察者
    subject_register_observer(&subject, &observer);

    // 设置主题数据并通知观察者
    subject_set_data(&subject, (observer_data_t *)&data);

    // 移除观察者
    subject_remove_observer(&subject, &observer);

    return 0;
}
```
更多测试用例请查阅`/example/test-observer.c`
可使用如下指令执行
```bash
cd ./example
make
make run
#执行输出情况
注册观察者
observer_pm_data.lv = 0x12345678
&observer_pm_data= 0x4e255030
通知观察者
observer update
        observer_pm :0x 12345678
        *pm_data= 0x4e255030
注册观察者2
通知观察者
observer update2
observer update
        observer_pm :0x 5555
        *pm_data= 0x4e255030
通知观察者1
通知观察者
observer update2
通知观察者
observer update2

```

---

### **5. 注意事项**
1. **内存管理**：观察者与主题之间的数据引用关系需要明确，避免野指针或重复释放问题。
2. **线程安全**：在多线程环境下，需要考虑观察者链表的同步访问问题。
3. **链表实现**：文档中使用了 `mm_list` 实现链表，确保链表的插入与删除操作高效。

---

### **6. 总结**
观察者模式是一种强大的设计模式，特别适合实时事件驱动的应用场景。本实现通过纯 C 语言的结构体与函数指针，实现了轻量级、无堆内存开销的观察者模式，适用于资源受限的嵌入式开发。
