// #ifndef MESSAGE_DISPATCH_T
// #define MESSAGE_DISPATCH_T

// #include <string>
// #include <map>
// #include <tr1/functional>
// #include "google/protobuf/message.h"

// class tcp_connection_t;
// class tcp_network_t;

// class msg_dispatch_t {
// public:
// 	class cb_t {
// 	public:
// 		virtual const char *get_name() = 0;

// 		virtual void set_name() = 0;

// 		virtual void set_msg_cb(void *) = 0;

// 		virtual void set_net_msg_cb(void *) = 0;

// 		virtual void on_message(int sid, const google::protobuf::Message *msg) = 0;

// 		virtual void on_net_message(tcp_connection_t *conn, const google::protobuf::Message *msg) = 0;
// 	};

// 	template<typename T>
// 	class cbT_t : public cb_t {
// 	public:
// 		typedef std::tr1::function<void (int, const T&)> msg_cb_t;

// 		typedef std::tr1::function<void (tcp_connection_t *, const T&)> net_msg_cb_t;

// 	public:
// 		virtual const char *get_name() { return name_; }

// 		virtual void set_name(const char *name) { name_ = name; }

// 		virtual void set_msg_cb(void *cb) { msg_cb_ = (msg_cb_t *) cb; }

// 		virtual void set_net_msg_cb(void *cb) { net_msg_cb_ = (net_msg_cb_t *) cb; }

// 		virtual void on_message(int sid, const google::protobuf::Message *msg) {
// 			if (msg_cb_) {
// 				const T *tmsg = dynamic_cast<const T *> (msg);
// 				msg_cb_(sid, tmsg);
// 			}
// 		}

// 		virtual void on_net_message(tcp_connection_t *conn, const google::protobuf::Message *msg) {
// 			if (net_msg_cb_) {
// 				const T *tmsg = dynamic_cast<const T *> (msg);
// 				net_msg_cb_(conn, tmsg);
// 			}
// 		}

// 	public:
// 		msg_cb_t *msg_cb_;

// 		net_msg_cb_t *net_msg_cb_;

// 		const char *name_;
// 	};

// 	typedef std::map<int, cb_t *> msg_map_t;

// 	typedef std::map<const char *, int> name_map_t;

// 	typedef std::tr1::function<bool (tcp_connection_t *)> on_script_func_t;

// public:
// 	msg_dispatch_t(tcp_network_t *network);

// 	~msg_dispatch_t();

// 	int msg_id(const char *name);	

// 	const char *msg_name(int id);

// 	void on_message(tcp_connection_t *conn, int msgid, google::protobuf::Message *msg);

// 	bool on_script_func(tcp_connection_t *conn);

// 	template<typename T>
// 	void register_message(const char *name, const typename cbT_t<T>::msg_cb_t& cb);

// 	template<typename T>
// 	void register_net_message(const char *name, const typename cbT_t<T>::net_msg_cb_t& cb);

// 	void set_on_script_func(on_script_func_t func) { on_script_func_ = func; }

// private:
// 	tcp_network_t *network_;

// 	msg_map_t msgs_;

// 	name_map_t names_;

// 	on_script_func_t on_script_func_;
// };

// #endif

#ifndef MESSAGE_DISPATCH_T
#define MESSAGE_DISPATCH_T

#include <map>
#include <string>
#include <tr1/functional>
#include "google/protobuf/message.h"

class tcp_connection_t;
class tcp_network_t;

class msg_dispatch_t {
public:
	class cb_t {
	public:
		cb_t() {}

		virtual ~cb_t() {}

 		virtual void on_message(int sid, const google::protobuf::Message *msg) = 0;
	};

	template<typename T>
	class cbT_t : public cb_t {
	public:
		typedef std::tr1::function<void (int, const T&)> msg_cb_t;

	public:
		cbT_t(msg_cb_t *cb) { cb_ = cb; }

		virtual void on_message(int sid, const google::protobuf::Message *msg) {
			const T *tmsg = dynamic_cast<const T *> (msg);
			cb_(sid, tmsg);
		}

	public:
		msg_cb_t *cb_;
	};

	class cb_net_t {
	public:
		cb_net_t() {}

		virtual ~cb_net_t() {}

		virtual void on_message(tcp_connection_t *conn, const google::protobuf::Message *msg) = 0;
	};

	template<typename T>
	class cb_netT_t : public cb_net_t {
	public:
		typedef std::tr1::function<void (tcp_connection_t *, const T&)> msg_cb_t;

	public:
		cb_netT_t(msg_cb_t *cb) { cb_ = cb; }

		virtual void on_message(tcp_connection_t *conn, const google::protobuf::Message *msg) {
			const T *tmsg = dynamic_cast<const T *> (msg);
			cb_(conn, tmsg);
		}

	public:
		msg_cb_t *cb_;
	};

	typedef std::map<const google::protobuf::Descriptor *, cb_t *> msg_map_t;

	typedef std::map<const google::protobuf::Descriptor *, cb_net_t *> msg_net_map_t;

	typedef std::tr1::function<bool (tcp_connection_t *, const char *)> on_script_func_t;

public:
	msg_dispatch_t(tcp_network_t *network);

	~msg_dispatch_t();

	template<typename T>
	void register_message(const typename cbT_t<T>::msg_cb_t& cb) { msg_map_[T::descriptor()] = new(cbT_t<T>, cb); }

	template<typename T>
	void register_net_message(const typename cbT_t<T>::cb_netT_t& c) { msg_net_map_[T::descriptor()] = new(cb_netT_t<T>, cb); }

	void set_on_script_func(on_script_func_t func) { on_script_func_ = func; }

private:
	void on_message(tcp_connection_t *conn, google::protobuf::Message *msg);

	bool on_script_func(tcp_connection_t *conn, const char *name);

private:
	tcp_network_t *network_;

	msg_map_t msg_map_;

	msg_net_map_t msg_net_map_;

	on_script_func_t on_script_func_;
};

#endif