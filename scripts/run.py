from pathlib import Path
import hashlib,os,subprocess,sys
p=Path(__file__).resolve().parents[1];rom=p/'rom/Streets of Rage.bin'
if not rom.is_file():sys.exit('Place your legally obtained ROM at '+str(rom))
if hashlib.sha256(rom.read_bytes()).hexdigest()!='dd44f120446654bb91c448762f3e0cd0d9b034f35d0e3266a4dc34402ada95c0':sys.exit('ROM SHA-256 mismatch; aborted.')
env=os.environ.copy();env.pop('GENESIS_FORCE_INTERP',None);env['SOR_HEAVY_DIAG']='0'
d=p/'build-play';d.mkdir(exist_ok=True);env['SOR_PROFILE_DIR']=str(d)
sys.exit(subprocess.call([str(p/'build/sor1_visual_case4'),str(rom),*sys.argv[1:]],cwd=d,env=env))
