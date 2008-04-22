#!/usr/bin/perl -w

use strict;

# This program is slow but very handy.

open MSG, '<messages.h';

my %S;

my $lang = shift || '__SPANISH__';
my $is_in = 0;

while(<MSG>)
{
  if (/$lang/)
  {
    $is_in = 1 - $is_in
  }
  if ($is_in)
  {
    for my $prefix ('TXT_', 'HELP_CM_')
    {
      if ($is_in && /define\s+($prefix\w+)\s+(\".*?\")/)
      {
        die ("duplicated key")
          if exists $S{$1};
        $S{$1} = $2;
      }
    }
  }
}

close MSG;

open RC, '<dfd.rc.source';

while (<RC>)
{
  my $orig = $_;
  foreach my $key (sort { # sort by length of the key, in reversed order
                           my $max = length($a);
                           if (length($b) > $max)
                           {
                             $max = length($b);
                           }
                           $max - length($a) <=> $max - length ($b)
                         } (keys %S))
  {
    if ($orig =~ m/$key/)
    {
      $orig =~ s/$key/$S{$key}/;
    }
  }
  print $orig;
}

# To generate the intermediate file:
# my $f = "";
#
#
# while(<>)
# {
# # MENUITEM "&Acercar\t+        ", CM_VERZOOMMAS
# if (/\s*(CM_\w+),\s*(\".*?\")/)
# {
#   $f .=  "  #define HELP_$1 $2\n";
#   print "$1, HELP_$1\n"
# }
# else
# {
#   die "error";
# }
# }
# print "\n\n" . $f;
