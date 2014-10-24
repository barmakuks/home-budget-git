#!/usr/bin/env python
def CreateMessage(sender, to, subject, message_text):
  """Create a message for an email.

  Args:
    sender: Email address of the sender.
    to: Email address of the receiver.
    subject: The subject of the email message.
    message_text: The text of the email message.

  Returns:
    An object containing a base64url encoded email object.
  """
  message = MIMEText(message_text)
  message['to'] = to
  message['from'] = sender
  message['subject'] = subject
  return {'raw': base64.urlsafe_b64encode(message.as_string())}

def SendMessage(service, user_id, message):
  """Send an email message.
  Args:
    service: Authorized Gmail API service instance.
    user_id: User's email address. The special value "me"
    can be used to indicate the authenticated user.
    message: Message to be sent.

  Returns:
    Sent Message.
  """
  try:
    message = (service.users().messages().send(userId=user_id, body=message)
               .execute())
    print 'Message Id: %s' % message['id']
    return message
  except errors.HttpError, error:
    print 'An error occurred: %s' % error


import httplib2
import argparse
from apiclient.discovery import build
from oauth2client.client import flow_from_clientsecrets
from oauth2client.file import Storage
from oauth2client.tools import run
from oauth2client import tools

import base64
from email.mime.audio import MIMEAudio
from email.mime.base import MIMEBase
from email.mime.image import MIMEImage
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
import mimetypes
import os

from apiclient import errors

OAUTH_SCOPE = 'https://www.googleapis.com/auth/gmail.compose'
STORAGE = Storage('gmail.storage')

client_secret_file = 'client_secret-home.budget.001.json'

flow = flow_from_clientsecrets(client_secret_file, scope=OAUTH_SCOPE)
http = httplib2.Http()
credentials = STORAGE.get()
if credentials is None or credentials.invalid:
    credentials = run(flow, STORAGE, http=http)

http = credentials.authorize(http)

gmail_service = build('gmail', 'v1', http=http)

user_id = 'me'

msg = CreateMessage('home.budget.001@gmail.com', 'home.budget.001@gmail.com', 'Transaction#01', 'Transaction')
SendMessage(gmail_service, user_id, msg)
