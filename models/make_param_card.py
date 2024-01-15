template="""nevents= 1
avg-ic = on
afterburner = on
system-and-sqrts = {system_and_sqrts}
norm = {normalization}
centrality-def= entropy
centrality-low= {cent_low}
centrality-high= {cent_high}
nucleon-width= 0.96
tau-fs= {freestream_time}
trento-args= {trento_args}
grid-step=.1
hydro-args= stop=0.140 min=0.08 slope=1.1 curvature=-0.5 zetas_max=0.05 zetas_width=0.02 zetas_t0=0.180 iskip_t=4 iskip_xy=2
Tswitch= 0.151"""

syst_and_sqrts = ['AuAu200','PbPb2760','PbPb5020']
normalizations = {'AuAu200':6.1, 'PbPb2760': 13.9,'PbPb5020':18.4}
free_streaming_times = {'AuAu200':0.5, 'PbPb2760':1.2, 'PbPb5020':1.2}

centralities = ['00-05', '05-10', '10-20', '20-30', '30-40', '40-50']
trento_arguments = {'AuAu200':'Au Au -x 4.23 -p 0. -k 1.2 --ncoll',
                    'PbPb2760':'Pb Pb -x 6.4 -p 0. -k 1.2 --ncoll',
                    'PbPb5020':'Pb Pb -x 7.0 -p 0. -k 1.2 --ncoll'}

import os
os.system("mkdir ./new_inputs")
for systname in syst_and_sqrts:
    norm = normalizations[systname]
    tauf = free_streaming_times[systname]
    trento = trento_arguments[systname]
    for cent in centralities:
        cents = cent.split('-')
        cent_low, cent_high = int(cents[0]), int(cents[1])
        content = template.format(system_and_sqrts=systname,
                                  normalization=norm,
                                  cent_low=cent_low,
                                  cent_high=cent_high,
                                  freestream_time=tauf,
                                  trento_args=trento)
        with open(f"./new_inputs/input_{systname}_{cent}.dat",'w') as f:
            f.write(content)
