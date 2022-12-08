# 香肠派对辅助
## 版本 1.7.3
```diff
! 2022-12-8
```

![LICENSE](https://img.shields.io/badge/license-GPL2-green?style=flat-square&logo=appveyor)

# 已完成功能
> 载具
>- 名称
>- 距离
>- 血量
>- 油量

> 玩家
>- 名称
>- 距离
>- 血量
>- 能量
>- 心情值
>- 状态

> 自瞄
>- 头部
>- 胸部

> 内存
>- 人物变大
>- 枪械修改

> 设置
>- 显示帧数

#### 如何更新
 - 在main.hpp内修改
```C++
enum event_module_Pointer : INT64 //TODO:更新更改eventBase
{
	eventBase = 0x03917908,//此处为GameAssembly.dll偏移
}
```

## 加入我们:[内测Q群](https://jq.qq.com/?_wv=1027&k=l9rmwm8g)
## 联系邮箱 a1992724048@outlook.com
## © 2022 遂沫

[回到顶部](#readme)
