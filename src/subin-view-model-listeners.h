#ifndef SUBIN_VIEW_MODEL_LISTENERS_H
#define SUBIN_VIEW_MODEL_LISTENERS_H

typedef enum
{
   EVENT_UPDATE,
   EVENT_LAST
}Events;

typedef void (*callback) (void *, void *);

typedef struct Handlers 
{
    callback cb;
	void* data;
	struct Handlers *next;
}EventHandlers;

EventHandlers* subin_view_model_listeners_get (Events e);

void subin_view_model_listeners_register (Events e, callback cb, void *data) ;
void subin_view_model_listeners_init ();
void subin_view_model_listeners_destroy ();

#endif /* __SUBIN_VIEW_MODEL_LISTENERS */
