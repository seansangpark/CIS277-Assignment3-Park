#include "MessageInbox.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

void showInbox(const MessageInbox& inbox)
{
    vector<TextMessage> snapshot = inbox.messages();

    cout << "Count:     " << inbox.messageCount() << endl;
    cout << "Used:      " << inbox.usedTextBytes() << endl;
    cout << "Remaining: " << inbox.remainingTextBytes() << endl;

    if (snapshot.empty())
    {
        cout << "(empty)" << endl;
        return;
    }

    for (size_t i = 0; i < snapshot.size(); i++)
    {
        string status = "unread";
        if (snapshot[i].isRead)
        {
            status = "read";
        }

        cout << snapshot[i].id << "  " << snapshot[i].sender << "  "
             << snapshot[i].text << "  " << status << endl;
    }
}

int main()
{
    MessageInbox inbox(20);
    TextMessage result;

    cout << "Text Message Inbox" << endl << endl;
    cout << "Budget:           20 text bytes" << endl;
    showInbox(inbox);
    cout << "Empty list: head and tail are null." << endl << endl;

    result.id = 9;
    result.sender = "old sender";
    result.text = "old text";
    result.isRead = true;
    if (!inbox.read(101, result))
    {
        cout << "Empty inbox. read rejected." << endl;
        cout << "ID unchanged:     " << result.id << endl;
        cout << "Sender unchanged: " << result.sender << endl;
        cout << "Text unchanged:   " << result.text << endl;
        cout << "Read unchanged:   " << result.isRead << endl << endl;
    }

    if (!inbox.erase(101))
    {
        cout << "Empty inbox. erase rejected." << endl << endl;
    }

    if (!inbox.receive(0, "Maya", "Hi!"))
    {
        cout << "ID zero rejected." << endl;
    }
    if (!inbox.receive(101, "", "Hi!"))
    {
        cout << "Empty sender rejected." << endl;
    }
    if (!inbox.receive(101, "Maya", ""))
    {
        cout << "Empty text rejected." << endl;
    }
    if (!inbox.receive(101, "Maya", string(21, 'x')))
    {
        cout << "Text larger than the budget rejected." << endl;
        showInbox(inbox);
        cout << endl;
    }

    inbox.receive(101, "Maya", "Hi!");
    cout << "Receive 101 from Maya: Hi!" << endl;
    showInbox(inbox);
    cout << "One message: head and tail point at the same node." << endl << endl;

    inbox.receive(102, "Noah", "Meet at 6");
    cout << "Receive 102 from Noah: Meet at 6" << endl;
    showInbox(inbox);
    cout << endl;

    if (!inbox.receive(103, "Eli", "Bring notes"))
    {
        cout << "Receive 103 from Eli: Bring notes" << endl;
        cout << "Rejected. 11 more bytes would exceed the budget." << endl;
        showInbox(inbox);
        cout << endl;
    }

    if (!inbox.receive(101, "Other", "x"))
    {
        cout << "Duplicate ID rejected." << endl;
        showInbox(inbox);
        cout << endl;
    }

    inbox.read(101, result);
    cout << "Read 101" << endl;
    if (result.isRead)
    {
        cout << "Returned: " << result.id << "  " << result.sender << "  "
             << result.text << "  read" << endl;
    }
    cout << "The message stays stored." << endl;
    showInbox(inbox);
    cout << endl;

    inbox.read(101, result);
    cout << "Read 101 again. Budget stays the same." << endl;
    showInbox(inbox);
    cout << endl;

    result.id = 8;
    result.sender = "still";
    result.text = "here";
    result.isRead = true;
    if (!inbox.read(999, result))
    {
        cout << "Missing ID. read rejected." << endl;
        cout << "ID unchanged:     " << result.id << endl;
        cout << "Sender unchanged: " << result.sender << endl;
        cout << "Text unchanged:   " << result.text << endl;
        cout << "Read unchanged:   " << result.isRead << endl << endl;
    }

    if (!inbox.erase(999))
    {
        cout << "Missing ID. erase rejected." << endl;
        showInbox(inbox);
        cout << endl;
    }

    cout << "Before deleting the first message:" << endl;
    cout << "head -> [101: Hi! read] -> [102: Meet at 6 unread] -> nullptr" << endl;
    cout << "                               ^ tail" << endl << endl;

    inbox.erase(101);
    cout << "Delete 101. Released that node and reclaimed 3 text bytes." << endl;
    showInbox(inbox);
    cout << endl;

    cout << "After deleting the first message:" << endl;
    cout << "head -> [102: Meet at 6 unread] -> nullptr" << endl;
    cout << "             ^ tail" << endl << endl;

    inbox.receive(103, "Eli", "Bring notes");
    cout << "Receive 103 from Eli: Bring notes" << endl;
    cout << "Accepted. The inbox uses exactly 20 bytes." << endl;
    showInbox(inbox);
    cout << endl;

    cout << "After deleting 101 and receiving 103:" << endl;
    cout << "head -> [102: Meet at 6 unread] -> [103: Bring notes unread] -> nullptr" << endl;
    cout << "                                        ^ tail" << endl << endl;

    if (!inbox.receive(104, "Tess", "x"))
    {
        cout << "Inbox is full. receive rejected." << endl;
        showInbox(inbox);
        cout << endl;
    }

    MessageInbox links(30);
    links.receive(1, "A", "a");
    links.receive(2, "B", "bb");
    links.receive(3, "C", "ccc");
    links.receive(4, "D", "dddd");
    links.receive(5, "E", "eeeee");
    cout << "Five messages received." << endl;
    showInbox(links);
    cout << endl;

    links.erase(3);
    cout << "Middle message 3 deleted." << endl;
    showInbox(links);
    cout << endl;

    cout << "Before deleting the last message:" << endl;
    cout << "head -> [1: a] -> [2: bb] -> [4: dddd] -> [5: eeeee] -> nullptr" << endl;
    cout << "                                               ^ tail" << endl << endl;

    links.erase(5);
    links.receive(6, "F", "ffffff");
    cout << "Last message deleted, then 6 appended." << endl;
    showInbox(links);
    cout << endl;

    cout << "After deleting the last message and appending:" << endl;
    cout << "head -> [1: a] -> [2: bb] -> [4: dddd] -> [6: ffffff] -> nullptr" << endl;
    cout << "                                                ^ tail" << endl << endl;

    links.erase(1);
    cout << "First message deleted." << endl;
    showInbox(links);
    cout << endl;

    links.erase(6);
    links.erase(2);
    cout << "One message remains." << endl;
    showInbox(links);
    cout << endl;

    cout << "Before deleting the only message:" << endl;
    cout << "head -> [4: dddd] -> nullptr" << endl;
    cout << "tail ---^" << endl << endl;

    links.erase(4);
    cout << "Only message deleted." << endl;
    showInbox(links);
    cout << "head -> nullptr" << endl;
    cout << "tail ---^" << endl << endl;

    if (!links.erase(4))
    {
        cout << "Empty inbox. erase rejected." << endl << endl;
    }

    links.receive(7, "Long sender name not charged to text budget", string(30, 'z'));
    cout << "Appended after the only node was deleted. Sender length is not charged." << endl;
    showInbox(links);
    cout << endl;

    MessageInbox copies(12);
    string sender = "Alice";
    string text = "Original";
    copies.receive(11, sender, text);
    sender = "Changed sender";
    text = "Changed text";
    cout << "Caller strings changed after receive." << endl;
    showInbox(copies);
    cout << endl;

    vector<TextMessage> copy = copies.messages();
    copy[0].id = 999;
    copy[0].sender = "Edited";
    copy[0].text = "Edited text";
    copy[0].isRead = true;
    copy.clear();
    cout << "Snapshot was edited and cleared." << endl;
    showInbox(copies);
    cout << endl;

    copies.read(11, result);
    result.id = 777;
    result.sender = "Edited output";
    result.text.clear();
    result.isRead = false;
    cout << "Read result was edited." << endl;
    showInbox(copies);
    cout << endl;

    copies.receive(12, "Bob", "four");
    cout << "Remaining 4 bytes filled." << endl;
    showInbox(copies);
    cout << endl;

    copies.clear();
    cout << "Inbox cleared." << endl;
    showInbox(copies);
    cout << endl;

    copies.clear();
    cout << "Clear called again." << endl;
    showInbox(copies);
    cout << endl;

    result.id = 4;
    result.sender = "keep";
    result.text = "untouched";
    result.isRead = true;
    if (!copies.read(11, result))
    {
        cout << "Read after clear rejected." << endl;
        cout << "ID unchanged:     " << result.id << endl;
        cout << "Sender unchanged: " << result.sender << endl;
        cout << "Text unchanged:   " << result.text << endl;
        cout << "Read unchanged:   " << result.isRead << endl << endl;
    }

    copies.receive(11, "Alice", string(12, 'a'));
    cout << "Old ID reused after clear." << endl;
    showInbox(copies);
    cout << endl;

    copies.clear();
    copies.receive(11, "Maya", "Bye");
    copies.erase(11);
    copies.receive(11, "Maya", "Bye");
    cout << "Deleted ID received again." << endl;
    showInbox(copies);

    return 0;
}
