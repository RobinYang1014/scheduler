#ifndef ITEM_H
#define ITEM_H

struct Item {
    int     date;
    char    *sched;

    struct Item*  next;
};


#endif

