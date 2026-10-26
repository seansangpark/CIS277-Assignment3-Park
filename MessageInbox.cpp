#include "MessageInbox.h"

MessageInbox::MessageInbox(size_t maxTextBytes)
{
    head = nullptr;
    tail = nullptr;
    count = 0;
    usedBytes = 0;
    budget = maxTextBytes;
}

MessageInbox::~MessageInbox()
{
    clear();
}

bool MessageInbox::receive(size_t id, const std::string& sender, const std::string& text)
{
    if (id == 0 || sender.empty() || text.empty())
    {
        return false;
    }

    if (text.size() > remainingTextBytes())
    {
        return false;
    }

    if (findNode(id) != nullptr)
    {
        return false;
    }

    Node* node = new Node;
    node->message.id = id;
    node->message.sender = sender;
    node->message.text = text;
    node->message.isRead = false;
    node->next = nullptr;

    if (tail != nullptr)
    {
        tail->next = node;
    }
    else
    {
        head = node;
    }

    tail = node;
    count++;
    usedBytes += text.size();
    return true;
}

bool MessageInbox::read(size_t id, TextMessage& result)
{
    Node* node = findNode(id);
    if (node == nullptr)
    {
        return false;
    }

    result = node->message;
    result.isRead = true;
    node->message.isRead = true;
    return true;
}

bool MessageInbox::erase(size_t id)
{
    Node* previous = nullptr;
    Node* current = head;

    while (current != nullptr && current->message.id != id)
    {
        previous = current;
        current = current->next;
    }

    if (current == nullptr)
    {
        return false;
    }

    Node* old = current;

    if (previous == nullptr)
    {
        head = old->next;
    }
    else
    {
        previous->next = old->next;
    }

    if (head == nullptr)
    {
        tail = nullptr;
    }
    else if (tail == old)
    {
        tail = previous;
    }

    usedBytes -= old->message.text.size();
    count--;
    delete old;
    return true;
}

std::vector<TextMessage> MessageInbox::messages() const
{
    std::vector<TextMessage> snapshot;

    for (Node* current = head; current != nullptr; current = current->next)
    {
        snapshot.push_back(current->message);
    }

    return snapshot;
}

void MessageInbox::clear()
{
    while (head != nullptr)
    {
        Node* old = head;
        head = old->next;
        delete old;
    }

    tail = nullptr;
    count = 0;
    usedBytes = 0;
}

size_t MessageInbox::messageCount() const
{
    return count;
}

size_t MessageInbox::usedTextBytes() const
{
    return usedBytes;
}

size_t MessageInbox::remainingTextBytes() const
{
    return budget - usedBytes;
}

MessageInbox::Node* MessageInbox::findNode(size_t id) const
{
    for (Node* current = head; current != nullptr; current = current->next)
    {
        if (current->message.id == id)
        {
            return current;
        }
    }

    return nullptr;
}
