#ifndef OPTIONS_H
#define OPTIONS_H

class Options {
    //Loop sounds when they complete?
    bool loop;
    //Only play when the trigger is pressed?
    bool remain_depressed;
    //When pressed again should it stop or restart?
    enum PressAgainAction { STOP, RESTART };
    PressAgainAction press_again;
};

#endif // OPTIONS_H
