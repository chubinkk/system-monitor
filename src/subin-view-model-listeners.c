#include "subin-common.h"
#include "subin-view-model-listeners.h"

static EventHandlers *listeners[EVENT_LAST];

EventHandlers *
subin_view_model_listeners_get (Events event)
{
    return listeners[event];
}

void
subin_view_model_listeners_register (Events event, callback cb, void* data)
{
    EventHandlers **handlers = &listeners[event];
    if (*handlers == NULL)
    {
        *handlers = (EventHandlers*) malloc(sizeof (EventHandlers));
        (*handlers)->cb = cb;
		(*handlers)->data = data;
        (*handlers)->next = NULL;
    }
    else
    {
	    while ((*handlers)->next != NULL) 
		{
		    (*handlers)++;
		}

        (*handlers)->next = (EventHandlers*) malloc(sizeof (EventHandlers));
		(*handlers)->next->cb = cb;
		(*handlers)->next->data = data;

		(*handlers)->next->next = NULL;
    }
}

void 
subin_view_model_listeners_destroy ()
{
    int i;
    EventHandlers *current, *next;
	for (i = 0; i < EVENT_LAST; i++)
	{
	    current = listeners[i];

		if (current == NULL)
		    continue;

		while (current)
		{
		    next = current->next;
			free (current);
			current = next;
		}
		listeners[i] = NULL;
	}
}

void
subin_view_model_listeners_init ()
{
    int i;
	for (i = 0; i < EVENT_LAST; i++)
	{
	    listeners[i] = NULL;
	}
}
