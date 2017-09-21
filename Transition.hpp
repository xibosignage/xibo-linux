#ifndef TRANSITION_HPP
#define TRANSITION_HPP

struct Transition
{
    enum Type
    {
        FadeIn,
        FadeOut,
        Fly
    } type;

    enum Direction
    {
        N,
        NE,
        E,
        SE,
        S,
        SW,
        W,
        NW
    } direction;

    int duration;
};


#endif // TRANSITION_HPP
