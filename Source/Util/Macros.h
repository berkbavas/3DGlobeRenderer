#pragma once

#define DEFINE_MEMBER(TYPE, NAME, ...)                           \
  protected:                                                     \
    TYPE m##NAME{__VA_ARGS__};                                   \
                                                                 \
  public:                                                        \
    inline const TYPE& Get##NAME(void) const { return m##NAME; } \
                                                                 \
  public:                                                        \
    inline TYPE& Get##NAME##_NonConst(void) { return m##NAME; }  \
                                                                 \
  public:                                                        \
    inline void Set##NAME(const TYPE& var) { m##NAME = var; }

#define DEFINE_MEMBER_CONST(TYPE, NAME, ...) \
  protected:                                 \
    TYPE m##NAME{__VA_ARGS__};               \
                                             \
  public:                                    \
    inline const TYPE& Get##NAME(void) const { return m##NAME; }

#define DISABLE_COPY(CLASS_NAME)                  \
    CLASS_NAME(const CLASS_NAME& other) = delete; \
    CLASS_NAME& operator=(const CLASS_NAME& other) = delete