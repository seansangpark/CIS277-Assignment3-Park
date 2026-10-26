# CIS-277 Assignment 3: Text Messages and Dynamic Memory

## Student

Sean Park

## Design

Each message is a node with a `TextMessage` and a `next` pointer. `MessageInbox` owns the nodes. `head` is the oldest message and `tail` is the newest. An empty list has both null. One message makes them the same node. `count` and `usedBytes` are stored on the inbox. `usedBytes` is the sum of `text.size()`. The sender is not added.

`receive` checks the id, sender, text, and remaining bytes before it allocates. The new node is filled, with `next` set to null, then linked from `tail`, or from `head` if the list is empty. `erase` and `clear` save the node, reconnect the live pointers, then `delete`. The destructor calls `clear`. Copying is deleted, so two inboxes do not own one chain.

## How to Compile

```bash
g++ -std=c++23 -Wall -Wextra -pedantic main.cpp MessageInbox.cpp -o message_inbox
```

## How to Run

```bash
./message_inbox
```

## Tests and Results

| Case | Expected | Actual |
| --- | --- | --- |
| Empty read and erase | Rejected, output left as written | id 9, `old sender`, `old text`, read flag still set |
| Maya and Noah, then Eli | used 12, Eli rejected | used 12, Eli rejected |
| Read 101, delete it, receive 103 | used 12, then 9, then 20 | used 12, then 9, then 20 |
| Invalid, duplicate, and missing ids | Inbox and output unchanged | Inbox and output unchanged |
| Delete middle, last, first, and only | Remaining ids stay in order | Order held, head and tail repaired |
| Edit the snapshot, then clear twice | Stored message unchanged, then empty | Alice / Original stayed, then count 0 both times |

`python3 validate.py --compiler g++ --sanitize` passed.

## Analysis Questions

1. Which pointers change when the first, last, or only message is deleted?

    The first message is `head`, so deleting it moves `head` to the next node. `tail` still points at the newest message. The last message is `tail`, so deleting it sets the previous node's next to null and moves `tail` back. `head` does not move. The only message is both ends, so both pointers become null. A middle message does not move `head` or `tail`. Only the previous node's next changes, so the list skips it.

    ```text
    Delete first:
    before  head -> [101] -> [102] -> nullptr
                             ^ tail
    after   head -> [102] -> nullptr
                    ^ tail

    Delete last:
    before  head -> [1] -> [2] -> [4] -> [5] -> nullptr
                                         ^ tail
    after   head -> [1] -> [2] -> [4] -> nullptr
                                  ^ tail

    Delete only:
    before  head -> [4] -> nullptr
            tail ---^
    after   head -> nullptr
            tail ---^
    ```

2. Why does reading leave storage budget unchanged while deleting makes room?

    `read` copies the message out and marks it read, but the node stays in the list, so `usedBytes` does not change. `erase` takes the node out and `delete` frees it, so those bytes are available again.

3. What does the byte counter measure, and which actual memory costs are excluded?

    `usedBytes` is the sum of `text.size()` for messages still in the list. For these texts that is the character count, including spaces. The sender, id, read flag, next pointer, and the extra memory for the string and allocator are not included.

4. Why does receive take O(n) search work even though appending takes O(1) link changes? Account for string copying too.

    Updating the old tail's next pointer and moving `tail` is O(1). `receive` still walks the list to reject a duplicate id, so that search is O(n). Copying the sender and the text also takes time based on their lengths.

5. What goes wrong if nodes remain allocated after destruction, or two inbox objects share ownership of the same chain?

    Nodes left allocated after destruction leak, and that text cannot be reused. If two inboxes shared one chain, the first destructor would delete the nodes and the second would delete them again. Copying is deleted so each inbox owns its own chain.

## AI Use

Cursor was used to help write the inbox and the demonstration. I checked the result by compiling, running `./message_inbox`, and running `python3 validate.py --compiler g++ --sanitize`.

### Reflection / Decision: 
This assignment is a perfect example of using a singly linked list. `head` is the oldest message and `tail` is the newest, so an id search is O(n) and appending is O(1). `count` and `usedBytes` are updated when a node is added or removed. Reading leaves the node linked. Deleting reconnects the list, then frees the node. One decision made was to check the id, the sender, the text, and the remaining bytes before `receive` calls `new`. A rejected message never becomes a node, so `head`, `tail`, and `usedBytes` stay as they were.