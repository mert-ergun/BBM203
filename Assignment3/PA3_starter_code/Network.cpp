#include "Network.h"


Network::Network() {

}

struct Command {
    string name;
    vector<string> parameters;
};

void message_command(string senderID, string recieverID, int message_limit, string message, string sender_port, string receiver_port, vector<Client> &clients, int framenum) {
    // Find sender and receiver
    int sender_index = -1, receiver_index = -1;

    for (int i = 0; i < clients.size(); i++) {
        if (clients[i].client_id == senderID) {
            sender_index = i;
        } else if (clients[i].client_id == recieverID) {
            receiver_index = i;
        }
    }

    Client& sender = clients[sender_index];
    Client& receiver = clients[receiver_index];
    
    
        // Send message with one packet

        // Create application layer packet
        ApplicationLayerPacket* app_layer_packet = new ApplicationLayerPacket(0, sender.client_id, receiver.client_id, message);
        
        // Create transport layer packet
        TransportLayerPacket* transport_layer_packet = new TransportLayerPacket(1, sender_port, receiver_port);

        // Create network layer packet
        NetworkLayerPacket* network_layer_packet = new NetworkLayerPacket(2, sender.client_ip, receiver.client_ip);

        // Find next hop
        string next_hop_id = sender.routing_table[receiver.client_id];
        Client next_hop = receiver;

        for (int i = 0; i < clients.size(); i++) {
            if (clients[i].client_id == next_hop_id) {
                next_hop = clients[i];
                break;
            }
        }

        // Create data link (physical) layer packet
        PhysicalLayerPacket* physical_layer_packet = new PhysicalLayerPacket(3, sender.client_mac, next_hop.client_mac);
        physical_layer_packet->num_hops = 0;

        // Add packets to frame stack
        stack<Packet*> frame;

        frame.push(app_layer_packet);
        frame.push(transport_layer_packet);
        frame.push(network_layer_packet);
        frame.push(physical_layer_packet);

        // Add frame to outgoing queue
        sender.outgoing_queue.push(frame);

        cout << "Frame #" << framenum << endl;
        physical_layer_packet->print();
        network_layer_packet->print();
        transport_layer_packet->print();
        app_layer_packet->print();
        cout << "Message chunk carried: \"" << message << "\"" << endl;
        cout << "Number of hops so far: " << physical_layer_packet->num_hops << endl;
        cout << "--------" << endl;    
}


void show_frame_info(string client_id, string queue_type, int frame_index, vector<Client> &clients) {
    Client client = Client(client_id, "", "");
    for (int i = 0; i < clients.size(); i++) {
        if (clients[i].client_id == client_id) {
            client = clients[i];
            break;
        }
    }

    stack<Packet*> frame;
    if (queue_type == "in") {

        if (frame_index > client.incoming_queue.size()) {
            cout << "No such frame." << endl;
            return;
        }

        queue <stack<Packet*>> temp_queue = client.incoming_queue;
        for (int i = 1; i < frame_index; i++) {
            temp_queue.pop();
        }
        frame = temp_queue.front();
        cout << "Current Frame #" << frame_index << " on the incoming queue of client " << client_id << endl; 

    } else if (queue_type == "out") {

        if (frame_index > client.outgoing_queue.size()) {
            cout << "No such frame." << endl;
            return;
        }

        queue <stack<Packet*>> temp_queue = client.outgoing_queue;
        for (int i = 1; i < frame_index; i++) {
            temp_queue.pop();
        }
        frame = temp_queue.front();
        cout << "Current Frame #" << frame_index << " on the outgoing queue of client " << client_id << endl;
    }

    Packet* layer3 = frame.top();
    PhysicalLayerPacket* physical_layer_packet = dynamic_cast<PhysicalLayerPacket*>(layer3);
    frame.pop();

    Packet* layer2 = frame.top();
    NetworkLayerPacket* network_layer_packet = dynamic_cast<NetworkLayerPacket*>(layer2);
    frame.pop();

    Packet* layer1 = frame.top();
    TransportLayerPacket* transport_layer_packet = dynamic_cast<TransportLayerPacket*>(layer1);
    frame.pop();

    Packet* layer0 = frame.top();
    ApplicationLayerPacket* application_layer_packet = dynamic_cast<ApplicationLayerPacket*>(layer0);
    frame.pop();

    cout << "Carried Message: \"" << application_layer_packet->message_data << "\"" << endl;

    cout << "Layer 0 info: ";  application_layer_packet->print();
    cout << "Layer 1 info: "; transport_layer_packet->print();
    cout << "Layer 2 info: "; network_layer_packet->print();
    cout << "Layer 3 info: "; physical_layer_packet->print();

    cout << "Number of hops so far: " << physical_layer_packet->num_hops << endl;
}


void show_q_info(string client_id, string queue_type, vector<Client> &clients) {
    Client client = Client(client_id, "", "");
    for (int i = 0; i < clients.size(); i++) {
        if (clients[i].client_id == client_id) {
            client = clients[i];
            break;
        }
    }

    if (queue_type == "in") {
        cout << "Client " << client_id << " Incoming Queue Status" << endl;
        cout << "Current total number of frames: " << client.incoming_queue.size() << endl;
    } else if (queue_type == "out") {
        cout << "Client " << client_id << " Outgoing Queue Status" << endl;
        cout << "Current total number of frames: " << client.outgoing_queue.size() << endl;
    }
}


void send_command(vector<Client> &clients) {
    // All frames in any client's outgoing queue are sent to the next hop
    // If the next hop is the receiver, the frame is added to the receiver's incoming queue
    // If the next hop is not the receiver, the frame is added to the next hop's incoming queue

    for (int i = 0; i < clients.size(); i++) {
        Client& client = clients[i];
        bool is_message_finished = false;
        int num_frames_sent = 1;


        while (!client.outgoing_queue.empty()) {
            stack<Packet*> temp = client.outgoing_queue.front();

            Packet* layer3 = temp.top();
            PhysicalLayerPacket* physical_layer_packet = dynamic_cast<PhysicalLayerPacket*>(layer3);
            temp.pop();

            Packet* layer2 = temp.top();
            NetworkLayerPacket* network_layer_packet = dynamic_cast<NetworkLayerPacket*>(layer2);
            temp.pop();

            Packet* layer1 = temp.top();
            TransportLayerPacket* transport_layer_packet = dynamic_cast<TransportLayerPacket*>(layer1);
            temp.pop();

            Packet* layer0 = temp.top();
            ApplicationLayerPacket* application_layer_packet = dynamic_cast<ApplicationLayerPacket*>(layer0);
            temp.pop();

            // Find next hop
            string next_hop_mac = physical_layer_packet->receiver_MAC_address;
            int next_hop_index = -1;

            for (int i = 0; i < clients.size(); i++) {
                if (clients[i].client_mac == next_hop_mac) {
                    next_hop_index = i;
                    break;
                }
            }

            Client& next_hop = clients[next_hop_index];

            // Add frame to next hop's incoming queue
            next_hop.incoming_queue.push(client.outgoing_queue.front());
            client.outgoing_queue.pop();

            string message = application_layer_packet->message_data;

            cout << "Client " << client.client_id << " sending frame #" << num_frames_sent << " to client " << next_hop.client_id << endl;
            physical_layer_packet->num_hops++;
            num_frames_sent++;
            physical_layer_packet->print();
            network_layer_packet->print();
            transport_layer_packet->print();
            application_layer_packet->print();
            cout << "Message chunk carried: \"" << message << "\"" << endl;
            cout << "Number of hops so far: " << physical_layer_packet->num_hops << endl;
            cout << "--------" << endl;


            if (message[message.length() - 1] == '.' || message[message.length() - 1] == '?' || message[message.length() - 1] == '!') {
                is_message_finished = true;
                num_frames_sent = 1;
            }
        }

    }
}


void receive_command(vector<Client> &clients) {
    // All frames in any client's incoming queue are received
    // If the next hop is the receiver, the frame is added to the receiver's incoming queue
    // If the next hop is not the receiver, the frame is added to the next hop's incoming queue

    for (int i = 0; i < clients.size(); i++) {
        Client& client = clients[i];
        bool is_message_finished = false;
        bool is_first_frame = true;
        int num_frames_received = 1;
        string total_message = "";

        while (!client.incoming_queue.empty()) {
            stack<Packet*> frame = client.incoming_queue.front();
            client.incoming_queue.pop();

            Packet* layer3 = frame.top();
            PhysicalLayerPacket* physical_layer_packet = dynamic_cast<PhysicalLayerPacket*>(layer3);
            frame.pop();

            Packet* layer2 = frame.top();
            NetworkLayerPacket* network_layer_packet = dynamic_cast<NetworkLayerPacket*>(layer2);
            frame.pop();

            Packet* layer1 = frame.top();
            TransportLayerPacket* transport_layer_packet = dynamic_cast<TransportLayerPacket*>(layer1);
            frame.pop();

            Packet* layer0 = frame.top();
            ApplicationLayerPacket* application_layer_packet = dynamic_cast<ApplicationLayerPacket*>(layer0);
            frame.pop();

            string receiver_id = application_layer_packet->receiver_ID;
            string receiver_mac = physical_layer_packet->receiver_MAC_address;
            string sender_id = application_layer_packet->sender_ID;
            string sender_mac = physical_layer_packet->sender_MAC_address;

            string hopper = client.client_id;

            for (int i = 0; i < clients.size(); i++) {
                if (clients[i].client_mac == sender_mac) {
                    hopper = clients[i].client_id;
                    break;
                }
            }

            string message = application_layer_packet->message_data;

            // Check if receiver is client
            if (client.client_id == receiver_id) {
                // Assemble message
                total_message += message;
                is_message_finished = false;

                if (message[message.length() - 1] == '.' || message[message.length() - 1] == '?' || message[message.length() - 1] == '!') {
                    is_message_finished = true;
                }

                cout << "Client " << client.client_id << " receiving frame #" << num_frames_received << " from client " << hopper << ", originating from client " << sender_id << endl;
                physical_layer_packet->print();
                network_layer_packet->print();
                transport_layer_packet->print();
                application_layer_packet->print();
                cout << "Message chunk carried: \"" << message << "\"" << endl;
                cout << "Number of hops so far: " << physical_layer_packet->num_hops << endl;
                cout << "--------" << endl;

                num_frames_received++;

                if (is_message_finished) {
                    if (is_message_finished) {
                        cout << "Client " << client.client_id << " received the message \"" << total_message << "\"" << " from client " << sender_id << "." << endl;
                        cout << "--------" << endl;

                        // Create log
                        // Get the current time
                        time_t now = time(0);

                        // Convert now to string form
                        char* dt = ctime(&now);

                        string timestamp = dt;

                        // Remove newline character from timestamp
                        timestamp = timestamp.substr(0, timestamp.length() - 1);

                        // Create log
                        Log log = Log(timestamp, total_message, num_frames_received - 1, physical_layer_packet->num_hops, sender_id, client.client_id, true, ActivityType::MESSAGE_RECEIVED);
                        client.log_entries.push_back(log);

                        total_message = "";
                    }

                    num_frames_received = 1;
                }

                // Delete packets
                delete application_layer_packet;
                delete transport_layer_packet;
                delete network_layer_packet;
                delete physical_layer_packet;
            } else {
                // Find next hop
                string next_hop_id = client.routing_table[receiver_id];
                Client next_hop = client;
                bool is_next_hop_valid = false;
                is_message_finished = false;

                for (int i = 0; i < clients.size(); i++) {
                    if (clients[i].client_id == next_hop_id) {
                        next_hop = clients[i];
                        is_next_hop_valid = true;
                        break;
                    }
                }

                // Check if next hop is not defined (X)
                if (!is_next_hop_valid) {
                    cout << "Client " << client.client_id << " receiving frame #" << num_frames_received << " from client " << hopper << ", but intended for client " << receiver_id << ". Forwarding... " << endl;

                    cout << "Error: Unreachable destination. Packets are dropped after " << physical_layer_packet->num_hops << " hops!" << endl;

                    num_frames_received++;
                    // Delete packets
                    if (message[message.length() - 1] == '.' || message[message.length() - 1] == '?' || message[message.length() - 1] == '!') {
                        // Create log
                        // Get the current time
                        time_t now = time(0);

                        // Convert now to string form
                        char* dt = ctime(&now);

                        string timestamp = dt;

                        // Remove newline character from timestamp
                        timestamp = timestamp.substr(0, timestamp.length() - 1);

                        // Create log
                        Log log = Log(timestamp, total_message, num_frames_received - 1, physical_layer_packet->num_hops, sender_id, receiver_id, false, ActivityType::MESSAGE_DROPPED);
                        client.log_entries.push_back(log);

                        is_message_finished = true;
                        is_first_frame = true;
                        num_frames_received = 1;
                        cout << "--------" << endl;
                    }

                    delete application_layer_packet;
                    delete transport_layer_packet;
                    delete network_layer_packet;
                    delete physical_layer_packet;
                    


                    continue;
                }

                for (int i = 0; i < clients.size(); i++) {
                    if (clients[i].client_id == next_hop_id) {
                        next_hop = clients[i];
                        break;
                    }
                }

                physical_layer_packet->receiver_MAC_address = next_hop.client_mac;
                physical_layer_packet->sender_MAC_address = client.client_mac;
                
                // Add all packets to stack
                stack<Packet*> new_frame;
                new_frame.push(application_layer_packet);
                new_frame.push(transport_layer_packet);
                new_frame.push(network_layer_packet);
                new_frame.push(physical_layer_packet);

                client.outgoing_queue.push(new_frame);
                
                if (num_frames_received > 1) {
                    is_first_frame = false;
                }

                if (is_first_frame) {
                    cout << "Client " << client.client_id << " receiving a message from client " << hopper << ", but intended for client " << receiver_id << ". Forwarding... " << endl;
                }
                

                // For every frame write "Frame #1 MAC address change: New sender MAC BBBBBBBBBB, new receiver MAC DDDDDDDDDD"
                cout << "Frame #" << num_frames_received << " MAC address change: New sender MAC " << client.client_mac << ", new receiver MAC " << next_hop.client_mac << endl;
                
                num_frames_received++;

                if (message[message.length() - 1] == '.' || message[message.length() - 1] == '?' || message[message.length() - 1] == '!') {
                    // Create log
                    // Get the current time
                    time_t now = time(0);

                    // Convert now to string form
                    char* dt = ctime(&now);

                    string timestamp = dt;

                    // Remove newline character from timestamp
                    timestamp = timestamp.substr(0, timestamp.length() - 1);

                    // Create log
                    Log log = Log(timestamp, total_message, num_frames_received - 1, physical_layer_packet->num_hops, sender_id, receiver_id, true, ActivityType::MESSAGE_FORWARDED);
                    client.log_entries.push_back(log);

                    is_message_finished = true;
                    is_first_frame = true;
                    num_frames_received = 1;
                    cout << "--------" << endl;

                }
            }

        }
    }
}


void print_log(vector<Client> &clients, string client_id) {
    for (int i = 0; i < clients.size(); i++) {
        if (clients[i].client_id == client_id) {
            if (clients[i].log_entries.size() == 0) {
                return;
            }
            cout << "Client " << client_id << " Logs:" << endl;
            for (int j = 0; j < clients[i].log_entries.size(); j++) {
                string ActivityTypeString;
                if (clients[i].log_entries[j].activity_type == ActivityType::MESSAGE_RECEIVED) {
                    ActivityTypeString = "Message Received";
                } else if (clients[i].log_entries[j].activity_type == ActivityType::MESSAGE_FORWARDED) {
                    ActivityTypeString = "Message Forwarded";
                } else if (clients[i].log_entries[j].activity_type == ActivityType::MESSAGE_SENT) {
                    ActivityTypeString = "Message Sent";
                } else if (clients[i].log_entries[j].activity_type == ActivityType::MESSAGE_DROPPED) {
                    ActivityTypeString = "Message Dropped";
                }

                cout << "--------------" << endl;
                Log log = clients[i].log_entries[j];
                cout << "Log Entry #" << j + 1 << ":" <<endl;
                cout << "Activity: " << ActivityTypeString << endl;
                cout << "Timestamp: " << log.timestamp << endl;
                cout << "Number of frames: " << log.number_of_frames << endl;
                cout << "Number of hops: " << log.number_of_hops << endl;
                cout << "Sender ID: " << log.sender_id << endl;
                cout << "Receiver ID: " << log.receiver_id << endl;
                cout << "Success: " << (log.success_status ? "Yes" : "No") << endl;

                if (ActivityTypeString == "Message Sent" || ActivityTypeString == "Message Received") {
                    cout << "Message: \"" << log.message_content << "\"" << endl;
                }

            }
            return;
        }
    }
}








void Network::process_commands(vector<Client> &clients, vector<string> &commands, int message_limit,
                      const string &sender_port, const string &receiver_port) {
    // TODO: Execute the commands given as a vector of strings while utilizing the remaining arguments.
    /* Don't use any static variables, assume this method will be called over and over during testing.
     Don't forget to update the necessary member variables after processing each command. For example,
     after the MESSAGE command, the outgoing queue of the sender must have the expected frames ready to send. */

    for (int i = 1; i < commands.size(); i++) {

        string command_string = "Command: " + commands[i];

        string command_dashes = "";
        for (int i = 0; i < command_string.length(); i++) {
            command_dashes += "-";
        }

        cout << command_dashes << endl;
        cout << command_string << endl;
        cout << command_dashes << endl;


        size_t start = 0;
        size_t end = commands[i].find(' ');
        Command cmd;

        cmd.name = commands[i].substr(start, end - start);

        if (cmd.name == "MESSAGE") {
            start = end + 1;
            size_t message_start = commands[i].find('#') + 1;
            string before_message = commands[i].substr(start, message_start - start - 2);
            size_t param_start = 0, param_end = 0;
            while ((param_end = before_message.find(' ', param_start)) != string::npos) {
                cmd.parameters.push_back(before_message.substr(param_start, param_end - param_start));
                param_start = param_end + 1;
            }
            cmd.parameters.push_back(before_message.substr(param_start));
            
            size_t message_end = commands[i].find('#', message_start);
            cmd.parameters.push_back(commands[i].substr(message_start, message_end - message_start));
        } else {
            start = end + 1;
            while ((end = commands[i].find(' ', start)) != string::npos) {
                cmd.parameters.push_back(commands[i].substr(start, end - start));
                start = end + 1;
            }
            if (start < commands[i].length()) {
                cmd.parameters.push_back(commands[i].substr(start));
            }
        }

        if (cmd.name == "MESSAGE") {
            // Get sender and receiver (sender is first word after command, receiver is second word after command)
            string senderID = cmd.parameters[0];
            string receiverID = cmd.parameters[1];
            string message = cmd.parameters[2];

            cout << "Message to be sent: \"" << message << "\"" << endl << endl;

            int frame = 1;

            for (size_t i = 0; i < message.length(); i+=message_limit) {
                string message_chunk = message.substr(i, message_limit);
                message_command(senderID, receiverID, message_limit, message_chunk, sender_port, receiver_port, clients, frame);
                frame++;
            }

            // Get the current time
            time_t now = time(0);

            // Convert now to string form
            char* dt = ctime(&now);

            string timestamp = dt;

            // Remove newline character from timestamp
            timestamp = timestamp.substr(0, timestamp.length() - 1);

            // Create log
            Log log = Log(timestamp, message, frame-1, 0, senderID, receiverID, true, ActivityType::MESSAGE_SENT);
            int sender_index = -1;

            // Find the sender
            for (int i = 0; i < clients.size(); i++) {
                if (clients[i].client_id == senderID) {
                    sender_index = i;
                }
            }

            Client& sender = clients[sender_index];

            sender.log_entries.push_back(log);

        } else if (cmd.name == "SHOW_FRAME_INFO") {
            // Show frame info of client_id
            string client_id = cmd.parameters[0];
            string queue_type = cmd.parameters[1];
            int frame_index = stoi(cmd.parameters[2]);

            show_frame_info(client_id, queue_type, frame_index, clients);
        } else if (cmd.name == "SHOW_Q_INFO") {
            // Show queue info of client_id
            string client_id = cmd.parameters[0];
            string queue_type = cmd.parameters[1];

            show_q_info(client_id, queue_type, clients);
        } else if (cmd.name == "SEND") {
            // Send all frames in any client's outgoing queue to the next hop
            send_command(clients);
        } else if (cmd.name == "RECEIVE") {
            // Receive all frames in any client's incoming queue
            receive_command(clients);
        } else if (cmd.name == "PRINT_LOG") {
            // Print log of client_id
            string client_id = cmd.parameters[0];

            print_log(clients, client_id);
        } else {
            cout << "Invalid command." << endl;
        }
    }
}

vector<Client> Network::read_clients(const string &filename) {
    vector<Client> clients;
    // TODO: Read clients from the given input file and return a vector of Client instances.

    string line;
    ifstream myfile(filename); 
    if (myfile.is_open()) {
        getline(myfile, line);  // skip first line
        while (getline(myfile, line)) {
            string id = line.substr(0, line.find(' '));
            string ip = line.substr(line.find(' ') + 1, line.find_last_of(' ') - line.find(' ') - 1);
            string mac = line.substr(line.find_last_of(' ') + 1);

            Client client(id, ip, mac);
            clients.push_back(client);
        }
        myfile.close();
    }
    this->clients = clients;

    return clients;
}

void Network::read_routing_tables(vector<Client> &clients, const string &filename) {
    // TODO: Read the routing tables from the given input file and populate the clients' routing_table member variable.

    string line;
    ifstream myfile(filename);
    if (myfile.is_open()) {
        // Until "-" character, that table is for that client
        int client_index = 0;
        while (getline(myfile, line)) {
            if (line == "-") {
                client_index++;
                continue;
            }

            string receiver_id = line.substr(0, line.find(' '));
            string next_hop_id = line.substr(line.find(' ') + 1);

            // Add routing table entry to the client
            clients[client_index].routing_table.insert({receiver_id, next_hop_id}); 
        }

        myfile.close();
    }
}

// Returns a list of token lists for each command
vector<string> Network::read_commands(const string &filename) {
    vector<string> commands;
    // TODO: Read commands from the given input file and return them as a vector of strings.

    string line;
    ifstream myfile(filename);
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            commands.push_back(line);
        }
        myfile.close();
    } 

    return commands;
}

Network::~Network() {
    // TODO: Free any dynamically allocated memory if necessary.
}
