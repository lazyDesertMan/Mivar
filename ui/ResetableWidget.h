#ifndef RESETABLE_WIDGET_H
#define RESETABLE_WIDGET_H

class ResetableWidget {
public:
    virtual void reset() = 0;
    virtual ~ResetableWidget() = default;
};

#endif
