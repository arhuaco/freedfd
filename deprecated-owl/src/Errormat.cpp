/*
    FreeDFD: Editor e intérprete de diagramas de flujo.
    Copyright (C) 1997-2006

    Nelson Castillo Izquierdo <arhuaco@freaks-unidos.net>
    Eduardo Daza Castillo <eduardodazacastillo@gmail.com>
    Fabián Cárdenas Varela <f_cardenas@yahoo.com>
                                    
    **********

    Necesitamos ayuda para terminar la nueva versión, que funcionará dentro
    de GNU/Linux y dentro de Windows, y que incluirá múltiples mejoras.
    Visítenos en:

    http://freedfd.freaks-unidos.net


 Español:

    Este programa es software libre. Puede redistribuirlo y/o modificarlo
    bajo los términos de la Licencia Pública General de GNU según es
    publicada por la Free Software Foundation, bien de la versión 2 de dicha
    Licencia o bien (según su elección) de cualquier versión posterior.

    Este programa se distribuye con la esperanza de que sea útil, pero SIN
    NINGUNA GARANTÍA, incluso sin la garantía MERCANTIL implícita o sin
    garantizar la CONVENIENCIA PARA UN PROPÓSITO PARTICULAR. Véase la
    Licencia Pública General de GNU para más detalles.

    Debería haber recibido una copia de la Licencia Pública General junto
    con este programa. Si no ha sido así, escriba a la Free Software
    Foundation, Inc., en 675 Mass Ave, Cambridge, MA 02139, EEUU.

 English:

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/



// Este handler evita que el programa se muera cuando se presentan
// algunas excepciones. Al mismo tiempo, permite dar mensajes más
// útiles al usuario.

#ifndef __ERRORMAT_CPP
#define __ERRORMAT_CPP


int _matherrl (struct _exceptionl *a)
{

	if( !strcmpi(a->name,"powl") )
		Buzon.SetIdentificadorAsociado( "^" );

	else if( !strcmpi(a->name,"sinl") )
		Buzon.SetIdentificadorAsociado( "SIN" );

	else if( !strcmpi(a->name,"cosl") )
		Buzon.SetIdentificadorAsociado( "COS" );

	else if( !strcmpi(a->name,"tanl") )
		Buzon.SetIdentificadorAsociado( "TAN" );

	else if( !strcmpi(a->name,"acosl") )
		Buzon.SetIdentificadorAsociado( "ACOS" );

	else if( !strcmpi(a->name,"asinl") )
		Buzon.SetIdentificadorAsociado( "ASIN" );

	else if( !strcmpi(a->name,"atanl") )
		Buzon.SetIdentificadorAsociado( "ATAN" );


	else if( !strcmpi(a->name,"sinhl") )
		Buzon.SetIdentificadorAsociado( "SINH" );

	else if( !strcmpi(a->name,"coshl") )
		Buzon.SetIdentificadorAsociado( "COSH" );

	else if( !strcmpi(a->name,"tanhl") )
		Buzon.SetIdentificadorAsociado( "TANH" );

	else if( !strcmpi(a->name,"expl") )
		Buzon.SetIdentificadorAsociado( "EXP" );

	else if( !strcmpi(a->name,"logl") )
		Buzon.SetIdentificadorAsociado( "LN" );

	else if( !strcmpi(a->name,"log10l") )
		Buzon.SetIdentificadorAsociado( "LOG" );

	else if( !strcmpi(a->name,"sqrtl") )
		Buzon.SetIdentificadorAsociado( "SQRT" );

	else
		Buzon.SetIdentificadorAsociado( "????/MATHERRL" );

	Buzon.Error( LLAMADO_DE_FUNCION_NO_VALIDO );
	a->retval=1;
	return 1;
 }

 #endif
