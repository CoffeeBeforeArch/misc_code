// This is a simple singly linked list implemented in Java
// By: Nick from CoffeeBeforeArch

class LinkedList {
    // Node class for linked list
    static class Node {
        int data;
        Node next;

        // Constructor
        Node(int d){
            data = d;
            next = null;
        }
    }

    // Keep track of the head of the list
    Node head;

    // Method for inserting into the list
    public static LinkedList insert(LinkedList list, int data){
        // Create a new node based on the data
        Node new_node = new Node(data);
        new_node.next = null;

        // Handle case that the list is empty
        // Otherwise normal insertion
        if(list.head == null){
            list.head = new_node;
        } else{
            // Start traversal at head
            Node temp_node = list.head;

            // Walk until you find the last node that points to null
            while(temp_node.next != null){
                temp_node = temp_node.next;
            }

            // Insert the new node at the end of the list
            temp_node.next = new_node;
        }

        // Return the updated list
        return list;
    }

    // Method for deleting from the list
    public static LinkedList delete(LinkedList list){
        // Handle case where we delete from an empty list
        // Otherwise normal deletion
        if(list.head == null){
            return list;
        } else{
            // Track previous and current node
            Node prev = null;
            Node temp = list.head;

            // Iterate over list until we find the last item
            while(temp.next != null){
                prev = temp;
                temp = temp.next;
            }

            // Handle case where we are deleting the last list item
            if(prev == null){
                list.head = null;
            }else{
                // Just set the previous item to point to null
                prev.next = null;
            }
            return list;
        }
    }

    // Method for printing the list
    public static void printList(LinkedList list){
        // Print a dividing line
        for(int i = 0; i < 72; i++){
            System.out.print("-");
        }
        System.out.println();
        
        // Print out the list
        System.out.print("List:\t");
        Node temp = list.head;
        while(temp != null){
            System.out.print(temp.data + "\t");
            temp = temp.next;
        }
        System.out.println();

        // Print a dividing line
        for(int i = 0; i < 72; i++){
            System.out.print("-");
        }
        System.out.println();
    }

    public static void main(String[] args){
        // Create a new linked list
        LinkedList ll = new LinkedList();

        // Add some nodes
        for(int i = 0; i < 5; i++){
            ll = insert(ll, i * i);
            printList(ll);
        }

        // Now delete some nodes
        for(int i = 0; i < 5; i++){
            ll = delete(ll);
            printList(ll);
        }
    }


}
