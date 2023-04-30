# HttpsC
## A HTTP/HTTPS Server in C
There is not a lot of this working right now.\
These things *are* working (mostly):
> **An HTTP Request Parser**
>> this ignores every Header in the request that has not been implemented (a tradeoff for fast lookup), because you don't get a Header by name but by index (which is essentially an O(1) lookup)

> *An HTTP Response formulator*
>> This takes in a HTTP_RESPONSE struct and gives a char* (aka string)