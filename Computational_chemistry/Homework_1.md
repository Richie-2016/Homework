# Introduction to Computational Chemistry
##### ChE62 钱睿 2016011846
---
## Chapter I
### 1.
请简要谈一谈你对理论与计算化学在现代化学中扮演的角色的认识，简而言之，这门学科有什么用处，意义何在？

##### 答：
计算化学是使用计算机模拟来解决化学问题的一门学科，主要是对于分子的结构、性质和行为进行预测和分析。这一学科有着它独特的不可替代的优势，即处理实验难以解决的复杂现象.

例如清华大学张强课题组在锂硫电池研究中发现，在锂硫电池系统中，对于复杂的化学反应和相关平衡关系的缺乏了解限制了研究的进展.他们认为，计算化学技术如高精度计算、材料基因组学等在锂硫体系中的应用有望推动进一步的研究 $^{[1]}$.

### 2.
请列举至少三位在量子力学发展过程中做出重要贡献的科学家，并简要介绍他们的贡献。

##### 答：

## Chapter II
### 1. 证明：
(1)厄米算符的本征值是实数;

(2)厄米算符的属于不同本征值的本征函数彼此正交.

##### 证明：
(1)对于厄米算符 $\hat F$, 有 $\mathinner{\langle \Psi |}\hat F\mathinner{|\varphi\rangle}=\mathinner{\langle \varphi |}\hat F\mathinner{|\Psi\rangle}^*$.

设函数 $\phi$是算符 $\hat F$的本征函数，本征值为 $\lambda$. 则有 $\hat F\mathinner{|\phi\rangle}\ =\ \lambda\mathinner{|\phi\rangle}$.

于是 $\mathinner{\langle \phi |}\hat F\mathinner{|\phi\rangle}=\mathinner{\langle \phi |}\lambda \mathinner{|\phi\rangle}=\lambda\mathinner{\langle \phi |}\mathinner{\phi\rangle}=\mathinner{\langle \phi |}\hat F\mathinner{|\phi\rangle}^*=\lambda^*\mathinner{\langle \phi |}\mathinner{\phi\rangle}$.

得到 $\lambda=\lambda^*$，$\lambda$是实数得证.

(2)设函数 $\Psi,\varphi$是算符 $\hat F$的属于不同本征值的本征函数，本征值分别为 $\lambda_1,\lambda_2$.

则有 $\mathinner{\langle \Psi |}\hat F\mathinner{|\varphi\rangle}=\lambda_1\mathinner{\langle \Psi |}\mathinner{\varphi\rangle}=\mathinner{\langle \varphi |}\hat F\mathinner{|\Psi\rangle}^*=\lambda_2\mathinner{\langle \Psi |}\mathinner{\varphi\rangle}$.

因为 $\lambda_1\neq\lambda_2$, 故 $\mathinner{\langle \Psi |}\mathinner{\varphi\rangle}=0$, 即这两个函数正交得证.

### 2.
请写出MgO的哈密顿算符.

##### 答：
$$
\hat H\ =\ -\frac{1}{2M_{Mg}}\nabla_{Mg}^2-\frac{1}{2M_O}\nabla_O^2-\sum_{i=1}^{20}(\frac{1}{2m}\nabla_i^2+\frac{12}{r_{Mg,i}}+\frac{8}{r_{O,i}})+\sum_{1\leq j<i\leq20}\frac{1}{r_{ij}}+\frac{96}{R_{MgO}}.
$$
