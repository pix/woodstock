#!/bin/sh

#installation script for woodstock (for system wide installs)
#Run as root!

[ -x /lib/woodstock.so ] && {
  touch /etc/ld.so.preload
  [ -w /etc/ld.so.preload ] && {
    [ `grep -c '/lib/woodstock.so'    /etc/ld.so.preload` == 0 ] && {
      echo     "/lib/woodstock.so" >> /etc/ld.so.preload
      echo "Installed"
    } || echo "woodstock already installed... updated to newest version"
  }   || echo "Failed, are you installing this as root?"
}     || echo "Failed, are you installing this as root?"
