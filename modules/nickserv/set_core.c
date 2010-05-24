/*
 * Copyright (c) 2006 William Pitcock, et al.
 * Rights to this code are documented in doc/LICENSE.
 *
 * This file contains routines to handle the CService SET command.
 *
 * $Id: set.c 7907 2007-03-06 23:10:26Z pippijn $
 */

#include "atheme.h"

DECLARE_MODULE_V1
(
	"nickserv/set", false, _modinit, _moddeinit,
	"$Id: set.c 7907 2007-03-06 23:10:26Z pippijn $",
	"Atheme Development Group <http://www.atheme.org>"
);

static void ns_cmd_set(sourceinfo_t *si, int parc, char *parv[]);

list_t *ns_cmdtree, *ns_helptree;

command_t ns_set = { "SET", N_("Sets various control flags."), AC_NONE, 2, ns_cmd_set };

list_t ns_set_cmdtree;

/* HELP SET */
static void ns_help_set(sourceinfo_t *si)
{
	command_success_nodata(si, _("Help for \2SET\2:"));
	command_success_nodata(si, " ");
	if (nicksvs.no_nick_ownership)
		command_success_nodata(si, _("SET allows you to set various control flags\n"
					"for accounts that change the way certain\n"
					"operations are performed on them."));
	else
		command_success_nodata(si, _("SET allows you to set various control flags\n"
					"for nicknames that change the way certain\n"
					"operations are performed on them."));
	command_success_nodata(si, " ");
	command_help(si, &ns_set_cmdtree);
	command_success_nodata(si, " ");
	command_success_nodata(si, _("For more information, use \2/msg %s HELP SET \37command\37\2."), nicksvs.nick);
}

/* SET <setting> <parameters> */
static void ns_cmd_set(sourceinfo_t *si, int parc, char *parv[])
{
	char *setting = parv[0];
	command_t *c;

	if (si->smu == NULL)
	{
		command_fail(si, fault_noprivs, _("You are not logged in."));
		return;
	}

	if (setting == NULL)
	{
		command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "SET");
		command_fail(si, fault_needmoreparams, _("Syntax: SET <setting> <parameters>"));
		return;
	}

	/* take the command through the hash table */
        if ((c = command_find(&ns_set_cmdtree, setting)))
	{
		command_exec(si->service, si, c, parc - 1, parv + 1);
	}
	else
	{
		command_fail(si, fault_badparams, _("Invalid set command. Use \2/%s%s HELP SET\2 for a command listing."), (ircd->uses_rcommand == false) ? "msg " : "", nicksvs.nick);
	}
}

void _modinit(module_t *m)
{
	MODULE_USE_SYMBOL(ns_cmdtree, "nickserv/main", "ns_cmdtree");
	MODULE_USE_SYMBOL(ns_helptree, "nickserv/main", "ns_helptree");
	command_add(&ns_set, ns_cmdtree);

	help_addentry(ns_helptree, "SET", NULL, ns_help_set);
}

void _moddeinit()
{
	command_delete(&ns_set, ns_cmdtree);
	help_delentry(ns_helptree, "SET");
}

/* vim:cinoptions=>s,e0,n0,f0,{0,}0,^0,=s,ps,t0,c3,+s,(2s,us,)20,*30,gs,hs
 * vim:ts=8
 * vim:sw=8
 * vim:noexpandtab
 */