#pragma once


#ifdef DEBUG
#define ERROR_MSG(msg)	printf(msg);
#else
#define ERROR_MSG()
#endif

#include <QString>

/// Function for loading dump
/**
 * Using streams. Load dump into memory and parse it.
 *   \param plGraph   - pointer to graph
 *   \param cDumpName - name of dump file
 *   \sa LGraph
 *   \return Returns 0 if success, 1 if file cannot open or allocate memory failed, 2 if dump corrupted.
 */
int ParseDump(LGraph * plGraph, const QString &filename);

