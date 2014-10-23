#!/usr/bin/env python

class MailInfo:
    date = ''
    sender = ''
    receiver = ''
    subject = ''

def getMail(client_secret_file):
    import httplib2
    import argparse
    from apiclient.discovery import build
    from oauth2client.client import flow_from_clientsecrets
    from oauth2client.file import Storage
    from oauth2client.tools import run
    from oauth2client import tools

    OAUTH_SCOPE = 'https://www.googleapis.com/auth/gmail.readonly'
    STORAGE = Storage('gmail.storage')

    flow = flow_from_clientsecrets(client_secret_file, scope=OAUTH_SCOPE)
    http = httplib2.Http()
    credentials = STORAGE.get()
    if credentials is None or credentials.invalid:
        credentials = run(flow, STORAGE, http=http)

    http = credentials.authorize(http)

    gmail_service = build('gmail', 'v1', http=http)

    user_id = 'me'

    threads = gmail_service.users().threads().list(userId=user_id).execute()

    mail_list = []
    if threads['threads']:
        for thread in threads['threads']:
            thread_id = thread['id']
            email = gmail_service.users().threads().get(userId=user_id, id=thread_id).execute()
            if email['messages']:
                for message in email['messages']:
                    mail = MailInfo();
                    for header in message['payload']['headers']:
                        if header['name'] == 'From':
                            mail.sender = header['value'].encode('utf-8')
                        elif header['name'] == 'To':
                            mail.receiver = header['value'].encode('utf-8')
                        elif header['name'] == 'Subject':
                            mail.subject = header['value'].encode('utf-8')
                        elif header['name'] == 'Date':
                            mail.date = header['value'].encode('utf-8')
                    mail_list.append(mail)
    return mail_list
