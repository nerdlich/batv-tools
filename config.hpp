#pragma once

#include "key.hpp"
#include <utility>
#include <netinet/in.h>
#include <map>
#include <vector>
#include <string>
#include <iosfwd>

namespace batv {
	struct Config {
		typedef std::pair<struct in6_addr, unsigned int> Ipv6_cidr;	// an IPv6 address and prefix length

		enum Failure_mode {
			FAILURE_TEMPFAIL,
			FAILURE_ACCEPT,
			FAILURE_REJECT
		};

		bool			daemon;
		int			debug;
		std::string		pid_file;
		std::string		socket_spec;
		int			socket_mode;		// or -1 to use the umask
		bool			do_sign;
		bool			do_verify;
		std::vector<Ipv6_cidr>	internal_hosts;		// we generate BATV addresses only for mail from these hosts
		Key_map			keys;			// map from sender address/domain to their HMAC key
		unsigned int		address_lifetime;	// in days, how long BATV address is valid
		char			sub_address_delimiter;	// e.g. "+"
		Failure_mode		on_internal_error;	// what to do when an internal error happens

		const Key*		get_key (const std::string& sender_address) const;	// Get HMAC key for the given sender
												// (NULL if sender doesn't use BATV)
		bool			is_internal_host (const struct in6_addr&) const;	// Is given IPv6 address internal?
		bool			is_internal_host (const struct in_addr&) const;		// Is given IPv4 addres internal?

		void			set (const std::string& directive, const std::string& value);
		void			load (std::istream&);
		void			validate () const;

		Config ()
		{
			daemon = false;
			debug = 0;
			socket_mode = -1;
			do_sign = true;
			do_verify = true;
			address_lifetime = 7;
			sub_address_delimiter = 0;
			on_internal_error = FAILURE_TEMPFAIL;
		}

	};
}

