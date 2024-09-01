#include <random>

#include "client.h"
#include "crypto.h"
#include "server.h"

Client::Client(std::string _id, const Server& _server):
    id(_id), server(&_server)
{
    std::string public_key, private_key;
    crypto::generate_key(public_key, private_key);
    this->public_key = public_key;
    this->private_key = private_key;
}

std::string Client::get_id() const
{
    return id;
}

std::string Client::get_publickey() const
{
    return public_key;
}

double Client::get_wallet()
{
   return const_cast<Server*>(server)->get_wallet(id);
}

std::string Client::sign(std::string txt) const
{
    return crypto::signMessage(private_key, txt);
}

bool Client::transfer_money(std::string receiver, double value)
{
    std::string _sign = id + "-" + receiver + "-" + std::to_string(value);
    return const_cast<Server*>(server)->add_pending_trx(_sign, sign(_sign));
}
	
size_t Client::generate_nonce()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution dis(0, 999999);
    return dis(gen);
}











