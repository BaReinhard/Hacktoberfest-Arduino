template <typename Function, typename... Arguments>
auto curry(Function func, Arguments... args)
{
    return [=](auto... rest) {
        return func(args..., rest...);
    };
}