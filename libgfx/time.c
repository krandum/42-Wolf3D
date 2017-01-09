/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: palatorr <palatorr@student.42.us.or>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/20 00:08:35 by palatorr          #+#    #+#             */
/*   Updated: 2016/10/20 00:08:36 by palatorr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgfx.h"

void	ft_get_time(struct timespec *ts)
{
#ifdef __MACH__
	clock_serv_t	cclock;
	mach_timespec_t	mts;

	host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
	clock_get_time(cclock, &mts);
	mach_port_deallocate(mach_task_self(), cclock);
	ts->tv_sec = mts.tv_sec;
	ts->tv_nsec = mts.tv_nsec;
#else
	clock_get_time(CLOCK_REALTIME, ts);
#endif
}
